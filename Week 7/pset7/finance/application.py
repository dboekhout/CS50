from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
from datetime import date, datetime

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    if request.method == "GET":
        # store user's id
        user_id = session.get("user_id")

        # get user portfolio
        stocks = db.execute("SELECT stock, symbol, amount FROM portfolio \
                            WHERE user_id = :user_id AND amount > 0",
                            user_id=user_id)

        # retrieve current cash balance
        cur_balance = db.execute("SELECT cash FROM users WHERE id = :id",
                                 id=user_id)

        # current stock prices
        cur_prices = []
        # amount of shares user owns times the current price
        sum_prices = []
        # holds the total net worth of the user's portfolio
        grand_total = cur_balance[0]["cash"]

        # loop over user's portfolio, calculating net worth (cash + stock value)
        for stock in stocks:
            quote = lookup(stock["symbol"])
            # append formatted price of stock
            cur_prices.append(quote.get("price"))
            # append stock price times number of shares
            sum_prices.append(quote.get("price") * stock["amount"])
            # add everything up to get the portfolio net worth
            grand_total += quote.get("price") * stock["amount"]

        return render_template("index.html",
                               stocks_owned=stocks,
                               stocks_prices=cur_prices,
                               total_stock_values=sum_prices,
                               current_balance=cur_balance[0]["cash"],
                               net_worth=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        # store user and stock information
        user_id = session.get("user_id")
        amount = request.form.get("amount")
        stock = lookup(request.form.get("stock"))

        # make sure stock is properly populated
        if not stock:
            error = "Couldn't get stock information"

        # make sure amount is not negative or empty
        elif not amount or int(amount) < 0:
            error = "Provide a positive share amount"

        # continue with purchase
        else:
            # price user has to pay for amount of shares
            cost = stock.get("price") * int(amount)

            # amount of cash a user has
            cash = db.execute("SELECT cash FROM users WHERE id=:id", id=user_id)

            # settle purchase
            new_cash = cash[0]["cash"] - cost

            # abort if user can't afford purchase
            if new_cash < 0:
                error = "You can't afford that"

            # reflect changes in database
            else:
                # add transaction to history
                db.execute("INSERT INTO transactions \
                           (user_id, symbol, amount, price, date) \
                           VALUES(:user_id, :symbol, :amount, :price, \
                           DATETIME('now','localtime'))",
                           user_id=user_id,
                           symbol=stock.get("symbol"),
                           amount=amount,
                           price=stock.get("price"))

                # update user's portfolio with new purchase
                db.execute("INSERT OR REPLACE INTO portfolio \
                           (user_id, stock, symbol, amount)\
                           VALUES (:user_id, :stock, :symbol, \
                           COALESCE((SELECT amount FROM portfolio \
                           WHERE user_id = :user_id AND symbol = :symbol), 0) \
                           + :amount);",
                           user_id=user_id,
                           stock=stock.get("name"),
                           symbol=stock.get("symbol"),
                           amount=amount)

                # update cash amount
                db.execute("UPDATE users SET cash = :new_cash WHERE id = :id",
                           new_cash=new_cash,
                           id=user_id)

                # purchase Successful
                flash("Shares Bought", "info")
                return redirect(url_for("index"))

        # inform user in case of error
        if error != None:
            flash(error, "danger")

    # page reached via get (include any errors that occurred)
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    if request.method == "GET":
        # gather transaction history for user
        history = db.execute("SELECT portfolio.stock,transactions.symbol,\
                             transactions.amount,transactions.price,\
                             transactions.date, transactions.action \
                             FROM transactions INNER JOIN portfolio \
                             ON portfolio.symbol = transactions.symbol \
                             WHERE transactions.user_id = :user_id \
                             GROUP BY transactions.transaction_id \
                             ORDER BY datetime(date) DESC;",
                             user_id=session.get("user_id"))

        return render_template("history.html", transactions=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""
    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # store form information
        username = request.form.get("username")
        password = request.form.get("password")

        # ensure username and password was submitted
        if not username or not password:
            error = "Must provide username/password"
        else:
            # query database for username
            rows = db.execute("SELECT * FROM users WHERE username = :username",
                              username=username.lower())

            # ensure username exists and password is correct
            if len(rows) != 1 or not \
                    pwd_context.verify(password, rows[0]["hash"]):
                error = "Wrong Username and/or Password"
            else:
                # remember which user has logged in
                session["user_id"] = rows[0]["id"]
                session["username"] = username

                # redirect user to home page
                flash("Login Successful", "success")
                return redirect(url_for("index"))

        # inform user in case of error
        if error != None:
            flash(error, "danger")

    # user reached via GET
    return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # retrieve current stock information
        quote = lookup(request.form.get("stock"))
        if quote == None:
            error = "Couldn't retrieve symbol information"
        else:
            return render_template("quoted.html",
                                   name=quote.get("name"),
                                   price=quote.get("price"),
                                   symbol=quote.get("symbol"))

        # inform user of any errors
        if error != None:
            flash(error, "danger")

    # user reached by GET request
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "POST":
        # store form information
        username = request.form.get("username").lower()
        password = request.form.get("password")
        pass_cnf = request.form.get("password-confirm")

        # validate form input
        if not username:
            error = "Must provide a username"
        elif not password or not pass_cnf:
            error = "Must provide a password and confirm it"
        elif password != pass_cnf:
            error = "Passwords don't match"

        # register user
        else:
            # safely store password and username in database
            hashed = pwd_context.hash(password)
            result = db.execute("INSERT INTO users (username, hash) \
                                VALUES(:username, :hash)",
                                username=username, hash=hashed)
            if not result:
                error = "Username already exists"
            else:
                # log user in automatically after registering
                session["user_id"] = result
                session["username"] = username

                flash("Successfully Created Account", "success")
                return redirect(url_for("index"))

        # inform user in case of error
        if error != None:
            flash(error, "danger")

    # user reached via GET request
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        # store user information
        user_id = session.get("user_id")
        symbol = request.form.get("stock").upper()
        amount = request.form.get("amount")

        # symbol field is empty
        if not symbol:
            error = "Must provide stock symbol"

        # amount is missing or negative
        elif not amount or int(amount) < 0:
            error = "Share amount can't be empty or negative"

        # continue selling
        else:
            # get stock to sell from user's portfolio
            portfolio = db.execute("SELECT stock, symbol, amount \
                                   FROM portfolio WHERE user_id = :user_id \
                                   AND symbol = :symbol",
                                   user_id=user_id,
                                   symbol=symbol)
            if not portfolio:
                error = "You don't have that stock in your portfolio"

            # abort if user is trying to sell more than available in portfolio
            elif int(amount) > portfolio[0].get("amount"):
                error = "You don't have that many shares"

            else:
                # store stock information
                stock = lookup(symbol)
                s_price = stock.get("price")

                # add transaction to history
                db.execute("INSERT INTO transactions (user_id, symbol, \
                           amount, price, date, action) \
                           VALUES(:user_id, :symbol, :amount, \
                           :price, DATETIME('now','localtime'), :action)",
                           user_id=user_id,
                           symbol=symbol,
                           amount=amount,
                           price=s_price,
                           action='SELL')

                # update user's portfolio to reflect sold shares
                db.execute("UPDATE portfolio SET amount = amount - :amount\
                           WHERE user_id = :user_id AND symbol = :symbol",
                           amount=amount,
                           user_id=user_id,
                           symbol=symbol)

                # amount of cash a user has
                cash = db.execute("SELECT cash FROM USERS WHERE id=:id",
                                  id=user_id)

                # settle sale
                new_cash = cash[0]["cash"] + (s_price * int(amount))

                # update cash amount
                db.execute("UPDATE users SET cash = :new_cash WHERE id = :id",
                           new_cash=new_cash,
                           id=user_id)

                flash("Shares Sold", "info")
                return redirect(url_for("index"))

        # inform user in case of error
        if error != None:
            flash(error, "danger")

    # user reached via GET request
    return render_template("sell.html")


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    if request.method == "POST":
        # user information and limits
        user_id = session.get("user_id")
        cash_limit = 5000
        dep_amount = 1000

        # amount of cash a user has
        cash = db.execute("SELECT cash FROM users WHERE id=:id", id=user_id)

        # check if user is eligible for deposit
        if cash[0]["cash"] > cash_limit:
            # deposit aborted, inform user
            info = "Seems like you have enough cash for the moment, \
            deposit canceled."
            flash(info, 'warning')
            return redirect(url_for("deposit"))
        else:
            # add 1000 dollars to users cash amount
            db.execute("UPDATE users SET cash = cash + :amount \
                       WHERE id = :user_id",
                       user_id=user_id, amount=dep_amount)

            # inform user of success
            success = "Added $1.000 dollars to your account, just like that!"
            flash(success, 'success')
            return redirect(url_for("deposit"))

    # user reached page via GET request
    return render_template("deposit.html")


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "POST":
        # store user and form information
        user_id = session.get("user_id")
        old_pass = request.form.get("old-password")
        new_pass = request.form.get("new-password")
        pass_cnf = request.form.get("password-confirm")

        # inform user of any passwords fields left empty
        if not old_pass or not new_pass or not pass_cnf:
            error = "Missing fields"
        else:
            # retrieve user's current password
            db_password = db.execute("SELECT hash FROM users WHERE id = :id",
                                     id=user_id)

            # old password doesn't match password in the database
            if not pwd_context.verify(old_pass, db_password[0]["hash"]):
                error = "Wrong Password"

            # new passwords don't match
            elif new_pass != pass_cnf:
                error = "New passwords don't match"

            # update the user's password in the database
            else:
                db.execute("UPDATE users SET hash = :hash WHERE id = :id",
                           hash=pwd_context.hash(new_pass),
                           id=session.get("user_id"))

                # successfully changed user's password
                flash("Successfully Changed Password", "success")
                return redirect(url_for("change_password"))

        # inform user in case of error
        if error != None:
            flash(error, "danger")

    # user reached via GET request
    return render_template("change_password.html")


@app.route("/delete_account", methods=["GET", "POST"])
@login_required
def delete_account():
    if request.method == "POST":
        # store user information
        password = request.form.get("password")
        user_id = session.get("user_id")

        # validate password
        if not password:
            error = "Provide your password to confirm account deletion"
        else:
            db_password = db.execute("SELECT hash FROM users WHERE id = :id",
                                     id=user_id)

            if not pwd_context.verify(password, db_password[0]["hash"]):
                error = "Wrong Password"

            else:
                # delete all references for user in database
                db.execute("DELETE FROM users WHERE id = :user_id",
                           user_id=user_id)
                db.execute("DELETE FROM transactions WHERE user_id = :user_id",
                           user_id=user_id)
                db.execute("DELETE FROM portfolio WHERE user_id = :user_id",
                           user_id=user_id)

                # redirect to home page after deletion
                session.clear()
                return redirect(url_for("login"))

        # inform user in case of error
        if error != None:
            flash(error, 'danger')

    return render_template("delete_account.html")

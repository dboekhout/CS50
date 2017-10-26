from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
from datetime import date, datetime
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

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///practical.db")

# routes
@app.route("/")
def index():
    return redirect(url_for("register"))

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register a group."""
    # if user reached route via POST (as by submitting in a form)
    if request.method == "POST":

        if request.form.get("group_name") == "":
            return apology("Fill in group name")
        elif request.form.get("student_1") == "":
            return apology("Fill in student 1")
        elif  request.form.get("student_2") == "":
            return apology("Fill in student 2")
        elif request.form.get("student_3") == "":
            return apology("Fill in student 3")
        elif request.form.get("student_4") == "":
            return apology("Fill in student 4")
        else:
            db.execute("INSERT INTO groups (group_name, student_1, student_2,\
             student_3, student_4, date) VALUES(:group_name, :student_1, \
             :student_2, :student_3, :student_4, :d_t)", \
             group_name=request.form.get("groupname"), \
             student_1=request.form.get("student_1"), \
             student_2=request.form.get("student_2"), \
             student_3=request.form.get("student_3"), \
             student_4=request.form.get("student_4"),\
             d_t=datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f"))
        # print(result)

        return redirect(url_for("overview"))


    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/overview")
def overview():
    """Gives an overview of the registered groups."""
    rows = db.execute("SELECT * FROM groups")
    return render_template("overview.html", rows=rows)

def apology(text=""):
    """Renders message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
            ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", text=escape(text))

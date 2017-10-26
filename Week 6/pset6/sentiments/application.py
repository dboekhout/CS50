# ----------------------------------------------------------
# |     University of Amsterdam: Minor Programmeren        |
# |     Problem Set 6: Sentiments (application)            |
# |     Created by Davy Boekhout on 10/10/2017             |
# |                                                        |
# |     Analyzes the tweets (max 100) of a given twitter   |
# |     handle and renders a chart using the flask         |
# |     web framework.                                     |
# ----------------------------------------------------------

import helpers
import os
import sys

from flask import Flask, redirect, render_template, request, url_for
from analyzer import Analyzer

app = Flask(__name__)


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/search")
def search():
    """Renders a chart that categorizes a Twitter user's timeline based on its
    sentiment.
    """
    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    # retrieve max 100 tweets from screen_name's timeline
    max_twts = 100
    tweets = helpers.get_user_timeline(screen_name, max_twts)
    if not tweets:
        return redirect(url_for("index"))

    # absolute paths to word lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    # initialize analyzer
    analyzer = Analyzer(positives, negatives)

    # calculate fractions for chart distribution
    positive, negative, neutral = 0.0, 0.0, 0.0

    # determine score for each tweet
    for tweet in tweets:
        score = analyzer.analyze(tweet)
        if score > 0:
            positive += 1
        elif score < 0:
            negative += 1
        else:
            neutral += 1

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)

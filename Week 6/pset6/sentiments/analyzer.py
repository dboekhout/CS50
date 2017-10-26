# ----------------------------------------------------------
# |     University of Amsterdam: Minor Programmeren        |
# |     Problem Set 6: Sentiments (analyzer)               |
# |     Created by Davy Boekhout on 10/10/2017             |
# |                                                        |
# |     Defines a sentiment analyzer. Takes in a positive  |
# |     and negative wordlist and uses heuristics to       |
# |     determine the sentiment of a given text.           |
# ----------------------------------------------------------

import nltk


class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        # open text files in universial read mode
        with open(positives, 'rU') as f_positives,\
                open(negatives, 'rU') as f_negatives:
            # list comprehensions that filter out whitespace and comments
            self.positives = [word for word in f_positives.read().splitlines()
                              if word and not word.startswith(";")]
            self.negatives = [word for word in f_negatives.read().splitlines()
                              if word and not word.startswith(";")]

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        # initialize score and tokenizer
        score = 0
        tknzr = nltk.tokenize.TweetTokenizer()
        tokns = tknzr.tokenize(text)

        # check if word in text is positive/negative and sum score accordingly
        for word in tokns:
            if word.lower() in self.positives:
                score += 1
            elif word.lower() in self.negatives:
                score -= 1
        return score

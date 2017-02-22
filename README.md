# hangman
A command-line hangman game with robust configuration options.

## Use:
hangman -d dictionaryfile [-n minimumletters] [-x maximumletters] [-p]

A dictionary file consists of a series of words, one on each line. The number of letters in the words selected can be modified with the -n and -x filtering options.

* When the game is running, typing a letter (and pressing ENTER to submit) guesses that letter.
* Entering = followed by a word guesses the entire word - an instant success or failure.
* Entering ! quits.
* Entering ? displays help text.
* Entering \ skips the current word and selects another.
* If compiled with DEBUG defined, entering /debug displays a debug report.

*Additional readme content coming soon*

Planned functionality:
* -w specifies maximum number of words to read
* -g specifies number of guesses before failure
* external configuration files
    

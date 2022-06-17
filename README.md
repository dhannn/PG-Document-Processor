# Project Gutenberg Document Processor

This program processes Project Gutenberg documents. It has two modes -- data cleaning and data analysis. 

Data cleaning ensures that the user's document is suitable for analysis. It does this by refining the document until the remaining elements are the most useful. Data analysis, on the other hand, involves determining key features of the text. This mode has two more sub-modes -- single-file analysis and multi-file analyis.

To learn more about the features, go to the [Features & Use Cases](#features--use-cases) section.

This project is programmed by Daniel Ramos and Gwen Roco in partial fulfillment for their **Programming with Structured Data Types** course.

# Requirements
1. C compiler
    * By default, make is configured with GCC.
    * If Clang is used, simply change the $(CC) variable.
2. Make

# Setup
## Screen
Make sure that the terminal is maximized.

If displays are not exhibiting expected behavior during runtime (i.e. elements displayed in the wrong places), simply resize the terminal and continue.

The program redraws the display every screen.

## ANSI Support

This program uses ANSI escape sequences which signals commands such as cursor positioning and color to the terminal.

However, some terminals don't support ANSI escape sequences.

### Command Prompt and Powershell
For these windows CLIs to support ANSI escape codes, follow these steps:

1. Go to Registry Editor.
2. Go to `HKEY_CURRENT_USER/Console`.
3. Search for the key `VirtualTerminalLevel`.
    * If not found, create a key.
5. Set the value to 1.

The key `HKEY_CURRENT_USER/Console/VirtualTerminalLevel` sets the default behavior of Windows terminals to support ANSI escape sequences.

### Windows Terminal
The new [Windows Terminal](https://www.microsoft.com/en-us/p/windows-terminal/9n0dx20hk701) supports ANSI escape code without adjustments.

# Features & Use Cases

## Data cleaning 
### To lowercase
This feature makes all the text lowercase.

### Special chaaracter removal
This feature removes punctuations and other non-alphanumeric characters.

### Numerical character removal
This feature removes numerical figures.

### Whitespace removal
This feature removes extra whitespaces (e.g., " ", "\n", "\t")

### Stopword removal
This feature removes common English words such as "the", "a" and "an" among others

## Data analyzer
### Single-file analysis

#### Word count
This feature counts the frequency of words in the text. Also known as term frequency.

#### N-gram count
This feature counts the occurrences of phrases with $n$ number of words (known as n-grams)

#### Concordance
This feature provides context by highlighting the surrounding words of a key term.

### Multi-file analysis
#### tf-idf
This feature is similar to the word count and n-gram count but it considers the relative term frequency of other documents in a corpus of text.

#### Document similarity (not implemented)
This feature computes the similarity of two documents either based on bag-of-words model (i.e. term frequency) or tf-idf

#### Document clustering (not implemented)
This feature assigns documents in a corpus to clusters either based on the bag-of-words model or tf-idf.

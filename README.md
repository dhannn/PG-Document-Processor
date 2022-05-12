# Project Gutenberg Document Processor

This program processes Project Gutenberg documents. It has two modes -- data cleaning and data analysis. 

Data cleaning ensures that the user's document is suitable for analysis. It does this by refining the document until the remaining elements are the most useful. Data analysis, on the other hand, involves determining key features of the text. This mode has two more sub-modes -- single-file analysis and multi-file analyis.

To learn more about the features, go to the [Features & Use Cases](#features--use-cases) section.

This project is programmed by Daniel Ramos and Gwen Roco in partial fulfillment for their **Programming with Structured Data Types** course.

# Requirements
1. C compiler
    * By default, make is configured with GCC.
    * If Clang is used, simply change the $(CC) variable.
3. Make

# Setup

# Installation

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

#### Document similarity
This feature computes the similarity of two documents either based on bag-of-words model (i.e. term frequency) or tf-idf

#### Document clustering
This feature assigns documents in a corpus to clusters either based on the bag-of-words model or tf-idf.

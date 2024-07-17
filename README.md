<h1 align="center">Super light integrated and power engine</h1>

<p align="center">

<img src="https://img.shields.io/badge/made%20by-silentlad-blue.svg" >

<img src="https://img.shields.io/npm/v/vue2-baremetrics-calendar">

<img src="https://img.shields.io/badge/vue-2.6.10-green.svg">

<img src="https://badges.frapsoft.com/os/v1/open-source.svg?v=103" >

<img src="https://img.shields.io/github/stars/silent-lad/Vue2BaremetricsCalendar.svg?style=flat">

<img src="https://img.shields.io/github/languages/top/silent-lad/Vue2BaremetricsCalendar.svg">

<img src="https://img.shields.io/github/issues/silent-lad/Vue2BaremetricsCalendar.svg">

<img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat">

<img src="https://badgen.net/#static/leanguage/C++/cyan">
</p>

<p align="center">
  <img src="./readme_assets/SearchEngine.png" width="100%">
</p>

# Console Search Engine

The Console Search Engine is a program designed for searching and indexing data. It is based on three classes: `ConverterJSON`, `InvertedIndex`, and `SearchServer`.

## Features

- **Data Search**: Enables searching for data based on specified criteria and parameters.
- **Indexing**: Performs data indexing for efficient and fast searching.
- **Configuration via JSON Files**: Provides the ability to configure the search engine using JSON files. JSON files contain configuration parameters such as data paths, indexing rules, and other settings.
- **Flexible Command-Line Interface**: Offers a convenient and intuitive command-line interface for interacting with the search engine.

## Classes

1. **ConverterJSON**: The `ConverterJSON` class is responsible for reading and converting data from JSON files. It provides methods for retrieving queries and other parameters from JSON files used for configuring the search engine.

2. **InvertedIndex**: The `InvertedIndex` class performs data indexing to facilitate efficient searching. It constructs an inverted index that maps words or terms to their corresponding documents or records. The class provides methods for building the index and executing search queries.

3. **SearchServer**: The `SearchServer` class represents the search server that combines the functionalities of the `ConverterJSON` and `InvertedIndex` classes. It handles user queries using the index created by `InvertedIndex` and provides search results.

## Installation and Usage

1. **Download the Repository**: Download the repository containing the search engine from GitHub.

2. **Configure File Paths**: Check and configure the file paths in the program. Depending on the Integrated Development Environment (IDE) used, the paths to the executable file may differ. Ensure that the paths to the data files, such as the document database and query files, are correctly specified.

3. **Add the Document Database**: Create a `.txt` file in the `resources` folder to serve as the document database. Update the path to this file in the configuration file (`config.json`).

4. **Run and Use**: Include the necessary header files (`converter_json.h`, `inverted_index.h`, and `search_server.h`) in your `main.cpp` file and compile the program. Execute the program to launch the console search engine.

Please note that this is a general description and you may need to provide more specific details and instructions depending on the structure and requirements of your project.

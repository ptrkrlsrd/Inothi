![](http://helvetia.design/INOTHI.png)

A simple cli program for writing short notes written in C++

<div id="table-of-contents">
<h2>Table of Contents</h2>
<div id="text-table-of-contents">
<ul>
<li><a href="#sec-1">1. Inothi</a>
<ul>
<li><a href="#sec-1-1">1.1. Features</a></li>
<li><a href="#sec-1-2">1.2. Requirements</a></li>
<li><a href="#sec-1-3">1.3. Installation</a></li>
<li><a href="#sec-1-4">1.4. Config</a>
<ul>
<li><a href="#sec-1-4-1">1.4.1. Example:</a></li>
</ul>
</li>
<li><a href="#sec-1-5">1.5. Usage</a>
<ul>
<li><a href="#sec-1-5-1">1.5.1. Available commands</a></li>
</ul>
</li>
</ul>
</li>
</ul>
</div>
</div>

## Features

-   Simple cli for creating notes
-   Adding tags
-   Readable output file

## Requirements

-   libconfig
-   boost

- [nhlohmann/json](https://github.com/nlohmann/json) 

## Installation

-   Get the required libraries
-   Clone this repo
-   Then run the following commands:
    -   make
    -   make config

## Config

### Example:

`path = "/Users/username/Notes/notes.json";`

## Usage

### Available commands

Most arguments should be familiar if you're using a unix based OS.

1.  Create a new note

    -   **Usage:** `inothi` (without any arguments)
    -   Enter the content of the note.
    -   Once you're done, hit enter.
    -   Next you'll be prompted to choose if you want to add tags or not.
        -   If you do, enter ***y*** if not enter ***n***.

2.  Delete a note (-rm)

    -   **Usage:** `inothi -rm`
    -   Select the note ID of the note you want to delete.

3.  List notes (-ls)

    -   **Usage:** `inothi -ls`

4.  Show available commands (--help)

    -   **Usage:** `inothi --help`

5.  Removes all notes (--clear)

    -   **Usage:** `inothi --clear`
    -   **Warning:** This will remove all your notes

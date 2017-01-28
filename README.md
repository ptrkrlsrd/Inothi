# Inothi

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
<li><a href="#sec-1-5-1">1.5.1. Create a new note</a></li>
<li><a href="#sec-1-5-2">1.5.2. List notes</a></li>
<li><a href="#sec-1-5-3">1.5.3. Delete a note</a></li>
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

## Installation

-   Get the required libraries
-   Clone this repo
-   Then run the following commands:
    -   make
    -   make config

## Config

### Example:

`path = "~/Notes/notes.txt";`

## Usage

Most arguments should be familiar if you're using a unix based OS.

### Create a new note

-   **Command:** `inothi` (without any arguments)
-   Enter the content of the note.
-   Once you're done, hit enter.
-   Next you'll be prompted to choose if you want to add tags or not.
    -   If you do, enter ***y*** if not enter ***n***.

### List notes

-   **Command:** `inothi -ls`

### Delete a note

-   **Command:** `inothi -rm`
-   Select the note ID of the note you want to delete.
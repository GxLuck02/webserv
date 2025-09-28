#!/usr/bin/env python3
import cgi
import cgitb
import os
import sys

# Enable debug (prints tracebacks in browser if error)
cgitb.enable()

UPLOAD_DIR = "/home/proton/Bureau/webserv/webserv/websites/upload"  # make sure this exists and is writable

def print_html(file_list, message=""):
    print("Content-Type: text/html")
    print()
    print("<!DOCTYPE html>")
    print("<html><head><title>CGI Gallery</title></head>")
    print("<body style='font-family: monospace; background: white; color: black;'>")
    print("<h1>Simple CGI Gallery</h1>")

    if message:
        print(f"<p><b>{message}</b></p>")

    # Upload form
    print("<h2>Upload a file</h2>")
    print("<form method='POST' enctype='multipart/form-data'>")
    print("<input type='file' name='file'>")
    print("<input type='submit' value='Upload'>")
    print("</form>")

    # File list with delete buttons
    print("<h2>Files</h2>")
    if not file_list:
        print("<p>No files found.</p>")
    else:
        print("<ul>")
        for fname in file_list:
            print(f"<li>{fname} "
                  f"<form method='POST' action='' style='display:inline;'>"
                  f"<input type='hidden' name='_method' value='DELETE'>"
                  f"<input type='hidden' name='filename' value='{fname}'>"
                  f"<input type='submit' value='Delete'>"
                  f"</form></li>")
        print("</ul>")

    print("</body></html>")


def handle_get():
    files = os.listdir(UPLOAD_DIR) if os.path.isdir(UPLOAD_DIR) else []
    print_html(files)


def handle_post():
    form = cgi.FieldStorage()

    # Fake DELETE (HTML forms do not support DELETE directly)
    if "_method" in form and form["_method"].value == "DELETE":
        filename = form["filename"].value
        filepath = os.path.join(UPLOAD_DIR, filename)
        if os.path.isfile(filepath):
            os.remove(filepath)
            message = f"File '{filename}' deleted."
        else:
            message = f"File '{filename}' not found."
        files = os.listdir(UPLOAD_DIR)
        print_html(files, message)
        return

    # Otherwise, handle file upload
    if "file" in form:
        field_item = form["file"]
        if field_item.filename:
            filename = os.path.basename(field_item.filename)
            filepath = os.path.join(UPLOAD_DIR, filename)
            with open(filepath, "wb") as f:
                f.write(field_item.file.read())
            message = f"File '{filename}' uploaded."
        else:
            message = "No file selected."
    else:
        message = "No file field in form."

    files = os.listdir(UPLOAD_DIR)
    print_html(files, message)


def handle_delete():
    # Direct DELETE request (non-form, e.g. curl)
    form = cgi.FieldStorage()
    if "filename" in form:
        filename = form["filename"].value
        filepath = os.path.join(UPLOAD_DIR, filename)
        if os.path.isfile(filepath):
            os.remove(filepath)
            message = f"File '{filename}' deleted."
        else:
            message = f"File '{filename}' not found."
    else:
        message = "No filename specified."

    files = os.listdir(UPLOAD_DIR)
    print_html(files, message)


def main():
    method = os.environ.get("REQUEST_METHOD", "GET")

    if not os.path.isdir(UPLOAD_DIR):
        os.makedirs(UPLOAD_DIR)

    if method == "GET":
        handle_get()
    elif method == "POST":
        handle_post()
    elif method == "DELETE":
        handle_delete()
    else:
        print("Status: 405 Method Not Allowed")
        print("Content-Type: text/plain\n")
        print("Method not allowed.")


if __name__ == "__main__":
    main()

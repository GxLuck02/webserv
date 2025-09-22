# #!/usr/bin/python3

# import cgi
# import cgitb
# import os
# import json
# import shutil
# from pathlib import Path

# # Activer le debug
# cgitb.enable()

# # Configuration
# UPLOAD_DIR = "/home/proton/Bureau/webserv/webserv/websites/upload"
# ALLOWED_EXTENSIONS = {'.jpg', '.jpeg'}

# def ensure_upload_dir():
#     """Creer le repertoire d'upload s'il n'existe pas"""
#     Path(UPLOAD_DIR).mkdir(parents=True, exist_ok=True)

# def is_allowed_file(filename):
#     """Verifier si l'extension du fichier est autorisee"""
#     return Path(filename).suffix.lower() in ALLOWED_EXTENSIONS

# def get_images():
#     """Recuperer la liste des images"""
#     images = []
#     if os.path.exists(UPLOAD_DIR):
#         for file in os.listdir(UPLOAD_DIR):
#             if is_allowed_file(file):
#                 images.append(file)
#     return images

# def handle_get():
#     """Gerer les requetes GET - afficher la galerie"""
#     images = get_images()
    
#     print("Content-Type: text/html\n")
    
#     html = """
#     <!DOCTYPE html>
#     <html>
#     <head>
#         <title>Galerie d'Images</title>
#         <style>
#             body {
#                 background-color: #2c2c2c;
#                 color: white;
#                 font-family: Arial, sans-serif;
#                 margin: 0;
#                 padding: 20px;
#                 display: flex;
#                 flex-direction: column;
#                 align-items: center;
#                 min-height: 100vh;
#             }
            
#             .upload-box {
#                 background: #3c3c3c;
#                 padding: 30px;
#                 border-radius: 15px;
#                 text-align: center;
#                 margin: 20px;
#                 width: 300px;
#                 box-shadow: 0 4px 8px rgba(0,0,0,0.3);
#             }
            
#             .delete-box {
#                 background: #3c3c3c;
#                 padding: 20px;
#                 border-radius: 10px;
#                 text-align: center;
#                 margin: 10px;
#                 width: 250px;
#                 box-shadow: 0 4px 8px rgba(0,0,0,0.3);
#             }
            
#             .gallery {
#                 display: flex;
#                 flex-wrap: wrap;
#                 justify-content: center;
#                 gap: 15px;
#                 margin-top: 30px;
#             }
            
#             .gallery img {
#                 width: 200px;
#                 height: 200px;
#                 object-fit: cover;
#                 border-radius: 10px;
#                 border: 2px solid #555;
#                 transition: transform 0.3s;
#             }
            
#             .gallery img:hover {
#                 transform: scale(1.05);
#                 border-color: #fff;
#             }
            
#             input[type="file"], input[type="text"] {
#                 background: #555;
#                 color: white;
#                 border: none;
#                 padding: 10px;
#                 border-radius: 5px;
#                 margin: 10px 0;
#                 width: 90%;
#             }
            
#             button {
#                 background: #007bff;
#                 color: white;
#                 border: none;
#                 padding: 12px 24px;
#                 border-radius: 8px;
#                 cursor: pointer;
#                 font-size: 16px;
#                 margin: 5px;
#                 transition: background 0.3s;
#             }
            
#             button:hover {
#                 background: #0056b3;
#             }
            
#             .delete-btn {
#                 background: #dc3545;
#             }
            
#             .delete-btn:hover {
#                 background: #c82333;
#             }
            
#             h1, h2 {
#                 margin: 10px 0;
#             }
#         </style>
#     </head>
#     <body>
#         <h1>Galerie d'Images</h1>
        
#         <div class="upload-box">
#             <h2>Upload</h2>
#             <form method="post" enctype="multipart/form-data">
#                 <input type="file" name="image" accept="image/*" required>
#                 <br>
#                 <button type="submit">Uploader</button>
#             </form>
#         </div>
        
#         <div class="delete-box">
#             <h2>Delete</h2>
#             <form onsubmit="event.preventDefault(); deleteImage()">
#                 <input type="text" id="filename" placeholder="Nom de l'image" required>
#                 <br>
#                 <button type="submit" class="delete-btn">Supprimer</button>
#             </form>
#         </div>
        
#         <div class="gallery">
#     """
    
#     for image in images:
#         html += '<img src="/upload/' + image + '" alt="' + image + '">'
    
#     html += """
#         </div>
        
#         <script>
#         function deleteImage() {
#             const filename = document.getElementById('filename').value;
#             if (!filename) {
#                 alert('Veuillez entrer un nom de fichier');
#                 return;
#             }
            
#             if (confirm('Supprimer ' + filename + ' ?')) {
#                 fetch(window.location.href, {
#                     method: 'DELETE',
#                     headers: {
#                         'Content-Type': 'application/json',
#                     },
#                     body: JSON.stringify({filename: filename})
#                 })
#                 .then(response => response.json())
#                 .then(data => {
#                     if (data.success) {
#                         alert('Image supprimee');
#                         location.reload();
#                     } else {
#                         alert('Erreur: ' + data.message);
#                     }
#                 })
#                 .catch(error => {
#                     alert('Erreur: ' + error);
#                 });
#             }
#         }
#         </script>
#     </body>
#     </html>
#     """
    
#     print(html)

# def handle_post():
#     """Gerer les requetes POST - upload d'images"""
#     ensure_upload_dir()
    
#     form = cgi.FieldStorage()
    
#     if "image" not in form:
#         print("Content-Type: text/html\n")
#         print('<script>alert("Aucun fichier selectionne"); window.location.href=".";</script>')
#         return
    
#     fileitem = form["image"]
    
#     if not fileitem.filename:
#         print("Content-Type: text/html\n")
#         print('<script>alert("Aucun fichier selectionne"); window.location.href=".";</script>')
#         return
    
#     if not is_allowed_file(fileitem.filename):
#         print("Content-Type: text/html\n")
#         print('<script>alert("Type de fichier non autorise"); window.location.href=".";</script>')
#         return
    
#     # Securiser le nom de fichier
#     filename = os.path.basename(fileitem.filename)
#     filepath = os.path.join(UPLOAD_DIR, filename)
    
#     try:
#         # Sauvegarder le fichier
#         with open(filepath, "wb") as f:
#             shutil.copyfileobj(fileitem.file, f)
        
#         print("Content-Type: text/html\n")
#         print('<script>alert("Image uploadee !"); window.location.href=".";</script>')
        
#     except Exception as e:
#         print("Content-Type: text/html\n")
#         print('<script>alert("Erreur lors du telechargement"); window.location.href=".";</script>')

# def handle_delete():
#     """Gerer les requetes DELETE - suppression d'images"""
#     try:
#         # Lire les donnees JSON
#         content_length = int(os.environ.get('CONTENT_LENGTH', 0))
#         if content_length > 0:
#             data = json.loads(sys.stdin.read(content_length))
#             filename = data.get('filename')
#         else:
#             raise ValueError("Aucune donnee recue")
        
#         if not filename:
#             raise ValueError("Nom de fichier manquant")
        
#         # Securiser le chemin
#         filename = os.path.basename(filename)
#         filepath = os.path.join(UPLOAD_DIR, filename)
        
#         if not os.path.exists(filepath):
#             raise FileNotFoundError("Fichier non trouve")
        
#         # Supprimer le fichier
#         os.remove(filepath)
        
#         print("Content-Type: application/json\n")
#         print(json.dumps({"success": True, "message": "Image supprimee"}))
        
#     except Exception as e:
#         print("Content-Type: application/json\n")
#         print(json.dumps({"success": False, "message": str(e)}))

# def main():
#     """Fonction principale"""
#     import sys
    
#     method = os.environ.get('REQUEST_METHOD', 'GET')
    
#     if method == 'GET':
#         handle_get()
#     elif method == 'POST':
#         handle_post()
#     elif method == 'DELETE':
#         handle_delete()
#     else:
#         print("Content-Type: text/html\n")
#         print("<h1>Methode " + method + " non supportee</h1>")

# if __name__ == "__main__":
#     main()

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

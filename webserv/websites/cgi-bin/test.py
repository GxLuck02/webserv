# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.py                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: proton <proton@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/20 18:06:26 by proton            #+#    #+#              #
#    Updated: 2025/09/20 18:14:56 by proton           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import cgi
import cgitb
import os
import json
import shutil
from pathlib import Path
import mimetypes
import urllib.parse

# Activer le debug CGI
cgitb.enable()

# Configuration
UPLOAD_DIR = "/var/www/html/images"  # Répertoire de stockage des images
ALLOWED_EXTENSIONS = {'.jpg', '.jpeg', '.png', '.gif', '.bmp', '.webp'}
MAX_FILE_SIZE = 5 * 1024 * 1024  # 5MB

def ensure_upload_dir():
    """Créer le répertoire d'upload s'il n'existe pas"""
    Path(UPLOAD_DIR).mkdir(parents=True, exist_ok=True)

def is_allowed_file(filename):
    """Vérifier si l'extension du fichier est autorisée"""
    return Path(filename).suffix.lower() in ALLOWED_EXTENSIONS

def get_images():
    """Récupérer la liste des images dans le répertoire"""
    images = []
    if os.path.exists(UPLOAD_DIR):
        for file in os.listdir(UPLOAD_DIR):
            if is_allowed_file(file):
                file_path = os.path.join(UPLOAD_DIR, file)
                file_size = os.path.getsize(file_path)
                images.append({
                    'name': file,
                    'size': file_size,
                    'url': f'/images/{file}'
                })
    return images

def handle_get():
    """Gérer les requêtes GET - afficher la galerie"""
    images = get_images()
    
    print("Content-Type: text/html; charset=utf-8\n")
    
    html = f"""
    <!DOCTYPE html>
    <html lang="fr">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Galerie d'Images</title>
        <style>
            body {{ font-family: Arial, sans-serif; margin: 20px; }}
            .upload-form {{ background: #f5f5f5; padding: 20px; border-radius: 8px; margin-bottom: 30px; }}
            .gallery {{ display: grid; grid-template-columns: repeat(auto-fill, minmax(300px, 1fr)); gap: 20px; }}
            .image-card {{ border: 1px solid #ddd; border-radius: 8px; padding: 15px; background: white; }}
            .image-card img {{ max-width: 100%; height: 200px; object-fit: cover; border-radius: 4px; }}
            .image-info {{ margin-top: 10px; }}
            .delete-btn {{ background: #dc3545; color: white; border: none; padding: 5px 10px; border-radius: 4px; cursor: pointer; }}
            .delete-btn:hover {{ background: #c82333; }}
            .upload-btn {{ background: #007bff; color: white; border: none; padding: 10px 20px; border-radius: 4px; cursor: pointer; }}
            .upload-btn:hover {{ background: #0056b3; }}
            .message {{ padding: 10px; margin: 10px 0; border-radius: 4px; }}
            .success {{ background: #d4edda; color: #155724; border: 1px solid #c3e6cb; }}
            .error {{ background: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }}
        </style>
    </head>
    <body>
        <h1>Galerie d'Images</h1>
        
        <div class="upload-form">
            <h2>Ajouter une image</h2>
            <form method="post" enctype="multipart/form-data">
                <input type="file" name="image" accept=".jpg,.jpeg,.png,.gif,.bmp,.webp" required>
                <button type="submit" class="upload-btn">Télécharger</button>
            </form>
        </div>
        
        <div class="gallery">
    """
    
    if images:
        for image in images:
            html += f"""
            <div class="image-card">
                <img src="{image['url']}" alt="{image['name']}" loading="lazy">
                <div class="image-info">
                    <strong>{image['name']}</strong><br>
                    Taille: {image['size']:,} octets
                    <br><br>
                    <button class="delete-btn" onclick="deleteImage('{image['name']}')">Supprimer</button>
                </div>
            </div>
            """
    else:
        html += "<p>Aucune image dans la galerie.</p>"
    
    html += """
        </div>
        
        <script>
        function deleteImage(filename) {
            if (confirm('Êtes-vous sûr de vouloir supprimer cette image ?')) {
                fetch(window.location.pathname, {
                    method: 'DELETE',
                    headers: {
                        'Content-Type': 'application/json',
                    },
                    body: JSON.stringify({filename: filename})
                })
                .then(response => response.json())
                .then(data => {
                    if (data.success) {
                        location.reload();
                    } else {
                        alert('Erreur: ' + data.message);
                    }
                })
                .catch(error => {
                    alert('Erreur lors de la suppression: ' + error);
                });
            }
        }
        </script>
    </body>
    </html>
    """
    
    print(html)

def handle_post():
    """Gérer les requêtes POST - upload d'images"""
    ensure_upload_dir()
    
    form = cgi.FieldStorage()
    
    if "image" not in form:
        print("Content-Type: text/html; charset=utf-8\n")
        print('<script>alert("Aucun fichier sélectionné"); window.location.href=window.location.pathname;</script>')
        return
    
    fileitem = form["image"]
    
    if not fileitem.filename:
        print("Content-Type: text/html; charset=utf-8\n")
        print('<script>alert("Aucun fichier sélectionné"); window.location.href=window.location.pathname;</script>')
        return
    
    if not is_allowed_file(fileitem.filename):
        print("Content-Type: text/html; charset=utf-8\n")
        print('<script>alert("Type de fichier non autorisé"); window.location.href=window.location.pathname;</script>')
        return
    
    # Sécuriser le nom de fichier
    filename = os.path.basename(fileitem.filename)
    filename = "".join(c for c in filename if c.isalnum() or c in ".-_")
    
    # Éviter les doublons
    filepath = os.path.join(UPLOAD_DIR, filename)
    counter = 1
    name, ext = os.path.splitext(filename)
    while os.path.exists(filepath):
        filename = f"{name}_{counter}{ext}"
        filepath = os.path.join(UPLOAD_DIR, filename)
        counter += 1
    
    try:
        # Vérifier la taille du fichier
        fileitem.file.seek(0, 2)  # Aller à la fin du fichier
        file_size = fileitem.file.tell()
        fileitem.file.seek(0)  # Retourner au début
        
        if file_size > MAX_FILE_SIZE:
            print("Content-Type: text/html; charset=utf-8\n")
            print('<script>alert("Fichier trop volumineux (max 5MB)"); window.location.href=window.location.pathname;</script>')
            return
        
        # Sauvegarder le fichier
        with open(filepath, "wb") as f:
            shutil.copyfileobj(fileitem.file, f)
        
        print("Content-Type: text/html; charset=utf-8\n")
        print('<script>alert("Image téléchargée avec succès"); window.location.href=window.location.pathname;</script>')
        
    except Exception as e:
        print("Content-Type: text/html; charset=utf-8\n")
        print(f'<script>alert("Erreur lors du téléchargement: {str(e)}"); window.location.href=window.location.pathname;</script>')

def handle_delete():
    """Gérer les requêtes DELETE - suppression d'images"""
    try:
        # Lire le contenu JSON de la requête
        content_length = int(os.environ.get('CONTENT_LENGTH', 0))
        if content_length > 0:
            raw_data = os.sys.stdin.buffer.read(content_length)
            data = json.loads(raw_data.decode('utf-8'))
        else:
            raise ValueError("Aucune donnée reçue")
        
        filename = data.get('filename')
        if not filename:
            raise ValueError("Nom de fichier manquant")
        
        # Sécuriser le nom de fichier
        filename = os.path.basename(filename)
        filepath = os.path.join(UPLOAD_DIR, filename)
        
        if not os.path.exists(filepath):
            raise FileNotFoundError("Fichier non trouvé")
        
        if not is_allowed_file(filename):
            raise ValueError("Type de fichier non autorisé")
        
        # Supprimer le fichier
        os.remove(filepath)
        
        print("Content-Type: application/json\n")
        print(json.dumps({"success": True, "message": "Image supprimée avec succès"}))
        
    except Exception as e:
        print("Content-Type: application/json\n")
        print(json.dumps({"success": False, "message": str(e)}))

def main():
    """Fonction principale"""
    method = os.environ.get('REQUEST_METHOD', 'GET')
    
    if method == 'GET':
        handle_get()
    elif method == 'POST':
        handle_post()
    elif method == 'DELETE':
        handle_delete()
    else:
        print("Content-Type: text/html; charset=utf-8\n")
        print(f"<h1>Méthode {method} non supportée</h1>")

if __name__ == "__main__":
    main()
    
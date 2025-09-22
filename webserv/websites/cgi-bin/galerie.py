

#!/usr/bin/env python3
import os
import sys

print("Content-Type: text/plain\r\n")  # Important : header CGI minimal
print("=== CGI DEBUG OUTPUT ===\n")

# --- 1. Affiche toutes les variables d'environnement ---
print(">>> Environment variables:")
for k, v in os.environ.items():
    print(f"{k} = {v}")
print("\n")

# --- 2. Vérifie s'il y a un body ---
method = os.environ.get("REQUEST_METHOD", "")
if method == "POST":
    length = os.environ.get("CONTENT_LENGTH")
    if length and length.isdigit():
        length = int(length)
        body = sys.stdin.read(length)   # lire exactement CONTENT_LENGTH octets
        print(">>> Body received (raw):")
        print(body)
    else:
        print(">>> No CONTENT_LENGTH or invalid, stdin may block.")
else:
    print(">>> No body (method = GET or DELETE)")


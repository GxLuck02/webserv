#!/usr/bin/env python3
import sys
import time

# CGI doit toujours commencer par un header vide entre "headers" et body
# Ici on triche un peu : on annonce explicitement Transfer-Encoding: chunked
sys.stdout.write("Content-Type: text/plain\r\n")
sys.stdout.write("Transfer-Encoding: chunked\r\n")
sys.stdout.write("\r\n")
sys.stdout.flush()

# Exemple de chunks envoyés progressivement
chunks = ["salut\n", "je\n", "teste\n", "le\n", "chunked\n"]

for part in chunks:
    data = part.encode("utf-8")
    size = format(len(data), "x")  # taille en hex
    sys.stdout.write(size + "\r\n")
    sys.stdout.write(part + "\r\n")
    sys.stdout.flush()
    time.sleep(1)  # pour voir le streaming

# chunk de fin
sys.stdout.write("0\r\n\r\n")
sys.stdout.flush()

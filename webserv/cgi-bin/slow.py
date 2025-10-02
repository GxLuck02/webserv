#!/usr/bin/env python3

import time
import sys

print("Content-Type: text/html\r")
print("\r")

print("<html><body>")
print("<h1>Script lent - debut</h1>")
print("</body></html>")

# Forcer l'envoi immédiat
sys.stdout.flush()

# Dormir 10 secondes (plus long que ton timeout de 5 sec)
time.sleep(10)

print("<html><body>")
print("<h1>Script lent - fin (ne devrait jamais apparaitre)</h1>")
print("</body></html>")
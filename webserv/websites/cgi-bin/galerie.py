#!/usr/bin/env python3

import os


def create_header_html():
	print("Content-Type: text/html\n")
	html = """<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Gestionaire d'image</title>
  <style>
	body {
		font-family: 'Segoe UI', sans-serif;
		height: auto;
		background-image: url('https://unblast.com/wp-content/uploads/2021/01/Space-Background-Image-8.jpg');
		background-size: cover;         /* Pour que l’image prenne tout l’espace */
		background-position: center;    /* Centrer l’image */
		background-repeat: no-repeat;   /* Ne pas répéter l’image */
		background-attachment: fixed;
			color: white;
		margin: 0px;
		padding: 0px;
			height: 100vh;
			position: relative;
	}

	h1 {
			color: #ffffff;
			margin-bottom: 20px;
			font-size: 2.2em;
		position: relative;
			text-shadow: 1px 1px 3px rgba(0,0,0,0.1);
		text-align: center;
		}

	form {
		margin: 30px auto;
		margin-top: 30px;
		width: 700px;
		background: #2e1d7a;
		padding: 25px 30px;
		border-radius: 15px;
		box-shadow: 0 4px 12px rgba(0,0,0,0.1);
		text-align: center;
		position: relative;
	}

	input[type="file"] {
		margin-bottom: 10px;
		margin-top: 15px;
		width: 370px;
		padding: 12px;
		font-size: 1.2rem;
		text-align: center;
		border-radius: 15px;
		background: #6f45be;
		color: white;
		cursor: pointer;
		border: none;
		transition: background 0.4s ease;
	}

	input[type="file"]:hover {
		background: #c50ef3;
	}

	.button_upload {
		margin-bottom: 15px;
		margin-right: 15px;
		width: 300px;
		padding: 15px;
		font-size: 1.2rem;
		background: #6f45be;
		border: none;
		color: white;
		text-align: center;
		border-radius: 15px;
		cursor: pointer;
		transition: background 0.4s ease;
	}

	.button_upload:hover {
		background: #c50ef3;
	}

	*{box-sizing:border-box}

	.slider-css-target{
		overflow: hidden;
		max-width: 1200px;
		margin: auto;
	}

	.slider-css-target ul{
		margin: 1rem .5rem;
		/*reset ul*/
		padding: 0;
		list-style-type:none;
		/*reset ul*/
		display:flex;
		justify-content: space-between;
		gap:.5rem
	}

	.slider-css-target ul li a{display:block}

	.slider-css-target ul li img{
		display: block;
		max-width: 100%;
		width: 120px;
		height: 100%;
		max-height: 80px;
		box-shadow:0 0 5px rgba(0, 0, 0, 0.3)
	}

	.slider-css-target-images {
		display: flex;
		width: 100%;
		transition: transform .4s ease-in-out;
	}

	.slider-css-target-images .description {
		margin-top: 15px;
		flex: 0 0 100%; /* 1 image par slide */
	}

	.slider-css-target-images img {
		display:block;
		width:100%;
		height: auto;
		margin-bottom: 30px;
	}

	#id1:target ~ .slider-css-target .slider-css-target-images{
		transform:translate3d(0,0,0)
	}

	#id2:target ~ .slider-css-target .slider-css-target-images{
		transform:translate3d(-25%,0,0)
	}

	#id3:target ~ .slider-css-target .slider-css-target-images{
		transform:translate3d(-50%,0,0)
	}

	#id4:target ~ .slider-css-target .slider-css-target-images{
		transform:translate3d(-75%,0,0)
	}

	.description{
		flex:0 1 100%;
		overflow: hidden;
		}

	.thumbnails-wrapper {
		display: flex;
		align-items: center;
		justify-content: center;
		gap: 10px;
		margin-top: 20px;
	}

	.thumbnails {
		display: flex;
		overflow: hidden;
		width: 520px;
		gap: 10px;
	}

	.thumbnails img {
		width: 120px;
		height: 80px;
		object-fit: cover;
		cursor: pointer;
		border-radius: 8px;
		box-shadow: 0 0 5px rgba(0,0,0,0.3);
		flex-shrink: 0;
		transition: transform 0.2s;
	}

	.thumbnails img:hover {
		transform: scale(1.05);
	}

	.thumb-nav {
		background: rgba(111,69,190,0.8);
		border: none;
		color: white;
		font-size: 1.5rem;
		padding: 5px 10px;
		cursor: pointer;
		border-radius: 8px;
		transition: background 0.3s;
	}

	.thumb-nav:hover {
		background: rgba(197,14,243,0.8);
	}

	.thumbnails img.active {
		border: 3px solid #c50ef3;
		transform: scale(1.1);
	}

</style>
</head>
	<body>
		<h1>✨ Quelle belle galerie photos ✨</h1>

		<form action="/upload" method="POST" enctype="multipart/form-data">
		<input type="file" name="image" accept="image/*" required>
		<button class="button_upload" type="submit">Uploader</button>
		</form>

		<div class="slider-css-target">

		<div class="thumbnails-wrapper">
			<button class="thumb-nav left">‹</button>
			<div class="thumbnails">"""
	print(html)

def create_galerie():
	images = os.listdir("websites/upload")
	for i, img in enumerate(images):
		name = os.path.splitext(img)[0]
		html = f'			<img src="/upload/{img}" alt="{name}" onclick="showImage({i})">'
		print(html);
	html = """		</div>
			<button class="thumb-nav right">›</button>
		</div>

		<div class="slider-css-target-images">"""
	print(html)
	for i, img in enumerate(images):
		name = os.path.splitext(img)[0]
		html = f'			<div class="description"><img src="/upload/{img}" alt="{name}"></div>'
		print(html)
	html = """		</div>

		</div>

	<script>
	  let currentImage = 0;
	  const slider = document.querySelector('.slider-css-target-images');
	  const thumbnails = document.querySelectorAll('.thumbnails img');
  
	  function showImage(index) {
		  currentImage = index;
		  const totalImages = slider.children.length;
		  slider.style.transform = `translateX(-${100 * currentImage}%)`;
		thumbnails.forEach((thumb, i) => {
			thumb.classList.toggle('active', i === currentImage);
		});
	}
	showImage(0);
	document.querySelector('.thumb-nav.left').onclick = () => {
		const wrapper = document.querySelector('.thumbnails');
		wrapper.scrollBy({ left: -130, behavior: 'smooth' });
	};
	document.querySelector('.thumb-nav.right').onclick = () => {
		const wrapper = document.querySelector('.thumbnails');
		wrapper.scrollBy({ left: 130, behavior: 'smooth' });
	};
	</script>
</body>
</html>"""
	print(html)

images = os.listdir("websites/upload")


create_header_html()
if (images):
	create_galerie()

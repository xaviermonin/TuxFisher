$(document).ready( function(){
	var video = $('#video');
	var taille = (video.width()/1.33);
	if (taille > 400)
		taille = 400;
	video.height(taille);
    $('#slider').nivoSlider();
});
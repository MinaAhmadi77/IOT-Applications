
# importing vlc module
import vlc
import time  
from serial import Serial
  
# creating vlc media player object
media_player = vlc.MediaPlayer()
arduino = Serial(port='COM4', baudrate=115200, timeout=.1)
  
# media object
media = vlc.Media("Rainbow.mp4")
media1 = vlc.Media("COD.mkv")
media_player.set_media(media1)
media_player.play()

media_player.video_set_scale(0.6)
volume = 50
media_player.audio_set_volume(volume)
while True:
    x = arduino.readline()
    if x.decode("utf-8") == "movie1\r\n":
        media_player.set_media(media)
        media_player.play()
    if x.decode("utf-8") == "movie2\r\n":
        media_player.set_media(media1)
        media_player.play()
    if x.decode("utf-8") == "pause\r\n":
        media_player.set_pause(1)
        
    if x.decode("utf-8") == "play\r\n":
        media_player.set_pause(0)
    if x.decode("utf-8") == "volumeUp\r\n":
        volume += 10
        media_player.audio_set_volume(volume)
    if x.decode("utf-8") == "volumeDown\r\n":
        volume -= 10
        media_player.audio_set_volume(volume)

        
        
    
         


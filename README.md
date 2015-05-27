# WeServd
Web server 

In Order to make the Webserver work, hit make run,
or hit make and then type: ./Webserver default_config

To get to the website:
localhost:1337/copycat
localhost:1337/images/image#.jpg
localhost:1337/html/assignment#.html

The way our webserver works is that they all call upon the common API, and then each handler returns a result.  This result is then used to write the response into the browser.
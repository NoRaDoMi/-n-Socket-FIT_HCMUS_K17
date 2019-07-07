﻿#pragma once

#include"HamBoTro.h"

// String hiển thị cho 403 Forbidden Error
string forbiddenResponse = "\n"
"\n"
"<!DOCTYPE html>\n"
"<html lang=\"en\">\n"
"<head>\n"
"<meta charset=\"UTF-8\">\n"
"<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"https://static.codepen.io/assets/favicon/favicon-aec34940fbc1a6e787974dcd360f2c6b63348d4b1f4e06c77743096d55480f33.ico\" />\n"
"<link rel=\"mask-icon\" type=\"\" href=\"https://static.codepen.io/assets/favicon/logo-pin-8f3771b1072e3c38bd662872f6b673a722f4b3ca2421637d5596661b4e2132cc.svg\" color=\"#111\" />\n"
"<title>403 Forbidden Page</title>\n"
"<style>\n"
"      @import url(\"https://fonts.googleapis.com/css?family=Bungee\");\n"
"\n"
"body {\n"
"  background: #1b1b1b;\n"
"  color: white;\n"
"  font-family: \"Bungee\", cursive;\n"
"  margin-top: 50px;\n"
"  text-align: center;\n"
"}\n"
"a {\n"
"  color: #2aa7cc;\n"
"  text-decoration: none;\n"
"}\n"
"a:hover {\n"
"  color: white;\n"
"}\n"
"svg {\n"
"  width: 50vw;\n"
"}\n"
".lightblue {\n"
"  fill: #444;\n"
"}\n"
".eye {\n"
"  cx: calc(115px + 30px * var(--mouse-x));\n"
"  cy: calc(50px + 30px * var(--mouse-y));\n"
"}\n"
"#eye-wrap {\n"
"  overflow: hidden;\n"
"}\n"
".error-text {\n"
"  font-size: 120px;\n"
"}\n"
".alarm {\n"
"  animation: alarmOn 0.5s infinite;\n"
"}\n"
"\n"
"@keyframes alarmOn {\n"
"  to {\n"
"    fill: darkred;\n"
"  }\n"
"}\n"
"\n"
"    </style>\n"
"<script>\n"
"  window.console = window.console || function(t) {};\n"
"</script>\n"
"<script>\n"
"  if (document.location.search.match(/type=embed/gi)) {\n"
"    window.parent.postMessage(\"resize\", \"*\");\n"
"  }\n"
"</script>\n"
"</head>\n"
"<body translate=\"no\">\n"
"<svg xmlns=\"http://www.w3.org/2000/svg\" id=\"robot-error\" viewBox=\"0 0 260 118.9\">\n"
"<defs>\n"
"<clipPath id=\"white-clip\"><circle id=\"white-eye\" fill=\"#cacaca\" cx=\"130\" cy=\"65\" r=\"20\" /> </clipPath>\n"
"<text id=\"text-s\" class=\"error-text\" y=\"106\"> 403 </text>\n"
"</defs>\n"
"<path class=\"alarm\" fill=\"#e62326\" d=\"M120.9 19.6V9.1c0-5 4.1-9.1 9.1-9.1h0c5 0 9.1 4.1 9.1 9.1v10.6\" />\n"
"<use xlink:href=\"#text-s\" x=\"-0.5px\" y=\"-1px\" fill=\"black\"></use>\n"
"<use xlink:href=\"#text-s\" fill=\"#2b2b2b\"></use>\n"
"<g id=\"robot\">\n"
"<g id=\"eye-wrap\">\n"
"<use xlink:href=\"#white-eye\"></use>\n"
"<circle id=\"eyef\" class=\"eye\" clip-path=\"url(#white-clip)\" fill=\"#000\" stroke=\"#2aa7cc\" stroke-width=\"2\" stroke-miterlimit=\"10\" cx=\"130\" cy=\"65\" r=\"11\" />\n"
"<ellipse id=\"white-eye\" fill=\"#2b2b2b\" cx=\"130\" cy=\"40\" rx=\"18\" ry=\"12\" />\n"
"</g>\n"
"<circle class=\"lightblue\" cx=\"105\" cy=\"32\" r=\"2.5\" id=\"tornillo\" />\n"
"<use xlink:href=\"#tornillo\" x=\"50\"></use>\n"
"<use xlink:href=\"#tornillo\" x=\"50\" y=\"60\"></use>\n"
"<use xlink:href=\"#tornillo\" y=\"60\"></use>\n"
"</g>\n"
"</svg>\n"
"<h1>FORBIDDEN PAGE</h1>\n"
"<h2> <a target=\"_blank\" href=\"https://www.google.com/\">GO HOME!</a></h2>\n"
"<script src=\"https://static.codepen.io/assets/common/stopExecutionOnTimeout-de7e2ef6bfefd24b79a3f68b414b87b8db5b08439cac3f1012092b2290c719cd.js\"></script>\n"
"<script id=\"rendered-js\">\n"
"      var root = document.documentElement;\n"
"var eyef = document.getElementById('eyef');\n"
"var cx = document.getElementById(\"eyef\").getAttribute(\"cx\");\n"
"var cy = document.getElementById(\"eyef\").getAttribute(\"cy\");\n"
"\n"
"document.addEventListener(\"mousemove\", evt => {\n"
"  let x = evt.clientX / innerWidth;\n"
"  let y = evt.clientY / innerHeight;\n"
"\n"
"  root.style.setProperty(\"--mouse-x\", x);\n"
"  root.style.setProperty(\"--mouse-y\", y);\n"
"\n"
"  cx = 115 + 30 * x;\n"
"  cy = 50 + 30 * y;\n"
"  eyef.setAttribute(\"cx\", cx);\n"
"  eyef.setAttribute(\"cy\", cy);\n"
"\n"
"});\n"
"\n"
"document.addEventListener(\"touchmove\", touchHandler => {\n"
"  let x = touchHandler.touches[0].clientX / innerWidth;\n"
"  let y = touchHandler.touches[0].clientY / innerHeight;\n"
"\n"
"  root.style.setProperty(\"--mouse-x\", x);\n"
"  root.style.setProperty(\"--mouse-y\", y);\n"
"});\n"
"      //# sourceURL=pen.js\n"
"    </script>\n"
"<script src=\"https://static.codepen.io/assets/editor/live/css_reload-5619dc0905a68b2e6298901de54f73cefe4e079f65a75406858d92924b4938bf.js\"></script>\n"
"</body>\n"
"</html>\n"
"";


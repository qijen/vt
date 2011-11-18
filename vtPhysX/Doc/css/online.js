function parser(fn)
{
var X, Y, sl, a, ra, link;
ra = /:/;
a = location.href.search(ra);
if (a == 2)
X = 14;
else
X = 7;
sl = "\\";
Y = location.href.lastIndexOf(sl) + 1;
link = 'file:///' + location.href.substring(X, Y) + fn;
location.href = link;
}


// fn     = filename
// width  = width
// height = height
// plugin = name of the plugin instance
// auto   = (TRUE, FALSE) autoplay
function parserCmo(fn,width,height,plugin,auto)
{
var X, Y, sl, a, ra, link;
ra = /:/;
a = location.href.search(ra);
if (a == 2)
X = 14;
else
X = 7;
sl = "\\";
Y = location.href.lastIndexOf(sl) + 1;
link = 'file:///' + location.href.substring(X, Y) + fn;
document.writeln('<object classid="clsid:C4925E65-7A1E-11D2-8BB4-00A0C9CC72C3" width="' + width + '" height="' + height + '" id="' + plugin + '">');
document.writeln('<param name="SRC" value="' + link +'">');
if(auto)
{
  document.writeln('<param name="AutoPlay" value="0">');
}
document.writeln('</object>');
}

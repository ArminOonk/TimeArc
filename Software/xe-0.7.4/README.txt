xe documentation

To install:
python setup.py install

For more information on Python distutils install:
http://docs.python.org/inst/



xe (short for "XML elements") is a Python library module designed to
make it easy to work with XML.  xe was originally designed as a simple
way to create XML, specifically for writing Atom syndication feeds, but
it has grown.

Once you have an xe class that represents an XML element, you have the
following advantages:

* You can get an XML format string representation of the element, either
  pretty-printed (nicely indented) or more terse.

* You can access the value directly, or get a string representation of
  it.  Likewise you can assign the value directly, or assign a string
  representation of the value (to the .text attribute).  For example, a
  IntElement has an integer value you can access directly by the .value
  attribute, and you can also access a .text attribute to get the
  integer as a string.

* The xe class can check that values make sense.  For example, if you
  have an element that represents a timestamp, and some code attempts to
  assign a string value that does not represent a valid timestamp, the
  timestamp class will raise an exception.

You can use xe as the base for a module that will, in some sense,
understand the semantics of the XML values you are reading and writing.
(You don't have to: as a quick-and-dirty way to work with some XML
values, you could just use TextElement for everything.)

There is support for parsing XML data into xe classes.  The xe element
classes have a method, .import_xml(), that accepts any remotely sensible
source of XML: a open file or file-like object, a filename, a URL, a DOM
node object, or a literal string containing XML.  For an example of how
this works, take a look at this example file: rss_parse_example.py

I have tried to do a good job with docstrings.  At a Python interpreter
prompt, if you run help() on any xe class, you should get some helpful
information.  If you find anyplace I need to improve the docstrings,
please email me.

The classes in xe just use the heck out of inheritance.  There are
abstract base classes that implement broadly useful behavior, and lots
of classes that just inherit and use this behavior; but there are plenty
of places where the child classes overload the inherited behavior and do
something different.  The way Python handles inheritance made this a joy
to code up.  Anyway, if you run help() on a class, it will show you all
the inherited methods too, so that's a good way to understand a class.

For processing arbitrary XML, xe may not be the best choice.  It is not
really designed for handling things like word processor document
formats, where you will have a soup of lots of simple text mixed with
attribute tags.  (However, if you really want to try it, xe should be
able to do it; look at the XMLText class.)


EXAMPLE OF USE

Suppose you had a simple Python program and you wanted to add a
preferences file to it. Initially, you just want a user name, and a
paper size. This small amount of code would work:


prefs = xe.NestElement("prefs")
prefs.user_name = xe.TextElement("user_name")
prefs.paper = xe.NestElement("paper")
prefs.paper.width = xe.IntElement("width")
prefs.paper.height = xe.IntElement("height")


This describes an XML data structure that contains a user name and a
paper size. Let's fill in some values:


prefs.user_name = "John Doe"
prefs.paper.width = 8
prefs.paper.height = 10

If you print it, or convert it to a string, the result will look like
this:

<prefs>
    <user_name>John Doe</user_name>
    <paper>
        <width>8</width>
        <height>10</height>
    </paper>
</prefs>



Instead of just printing it, you could save this in a file. Later, to
parse the file, you could just take your prefs variable and do this:

prefs.import_xml("preferences_file.xml")



You can get access to the value of width or height as an integer:

width.value == 8  # evaluates to True



For a real application, you would probably make a class called Prefs()
that would hold the preferences, rather than manually building a single
"prefs" instance as in the example above. Take a look at the test cases
at the end of xe.py, for a simple example.  Or, if you also have PyFeed,
take a look at atom.py to see how the Atom spec is implemented in xe.
It's actually quite simple!



If you have any questions, comments, or bug reports about xe,
please contact me using this email address:  xe@langri.com


I've had fun writing xe and I hope that you will find it useful.

Steve R. Hastings
xe@langri.com

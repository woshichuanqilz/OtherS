> Where should I start ? 
At first I found it's very **boring and time-consuming** to use the *windows  native search engine*. Like this : 
![08~08_10-37-06.bmp](http://7xpvdr.com1.z0.glb.clouddn.com/08~08_10-37-06.bmp)

So I want to find a software which is more faster. : ) 

# Search Files In Folder
## 1. Find Everything 

[Google Search Result](https://www.google.co.jp/webhp?sourceid=chrome-instant&ion=1&espv=2&ie=UTF-8#q=quick%20search%20file%20in%20windows)

--- 
### Tips
*The website I prefer to search for information:*
1. [zhihu](http://www.zhihu.com/)
![08~08_10-41-29.bmp](http://7xpvdr.com1.z0.glb.clouddn.com/08~08_10-41-29.bmp)
2. [Google](https://www.google.co.jp/)
![08~08_10-41-51.bmp](http://7xpvdr.com1.z0.glb.clouddn.com/08~08_10-41-51.bmp)
3. This is for coder :) in most **situation** [stackoverflow](http://stackoverflow.com/) 
![08~08_10-42-28.bmp](http://7xpvdr.com1.z0.glb.clouddn.com/08~08_10-42-28.bmp)

***And Forum for certain specialty***
1. Now just use for Vim [Google Group](https://groups.google.com/forum/#!myforums)
2. [Excel General](http://www.excelforum.com/)
3. [AutoHotkey](https://autohotkey.com/board/)




## 2.ShortCut to Active the Everything 
### [Search Result : shortcut to open programs windows](https://www.google.co.jp/webhp?sourceid=chrome-instant&ion=1&espv=2&ie=UTF-8#q=shortcut%20to%20open%20programs%20windows)
> I find a way about Use [Ctrl Alt {X}] to Active a program, it's hard to press.
![08~08_10-45-06.bmp](http://7xpvdr.com1.z0.glb.clouddn.com/08~08_10-45-06.bmp)

### So I modify the Search Key word to 
[Search result : customize hotkey for program in windows](https://www.google.co.jp/search?sclient=psy-ab&biw=1906&bih=914&noj=1&q=customize+hotkey+for+program+in+windows&oq=customize+hotkey+for+program+in+windows&gs_l=serp.3...31010.32134.1.32302.6.5.0.0.0.0.0.0..0.0....0...1c.1.64.serp..6.0.0.dCI81W8Xotw)


#### It is a little hard to find what you want in the Google search result 
We get a answer like **AHK**. Or We can ask the question in the *stackoverflow* .
And we can use the New hotkey `win + f` for the alternate
**AHK CODE**
```
#f::
{
    Run, C:\Program Files\Everything\Everything.exe
}
```

## 3. Search in the certain path
[Google result : Everything search in certain path](https://www.google.co.jp/search?sclient=psy-ab&biw=1906&bih=914&noj=1&q=Everything+search+in+the+certain+path&oq=Everything+search+in+the+certain+path&gs_l=serp.3...842051.852943.1.853075.82.37.11.0.0.0.502.4761.0j2j8j6j0j1.17.0....0...1c.1.64.serp..58.17.2414.0..0j46j0i67j35i39j0i3j0i3i46j46i3j0i10i67j0i20j0i46j0i22i30j0i22i10i30.DyVxkab73HU)

### And we get this :
[How do I search for files and folders in a specific location?](https://www.voidtools.com/faq/#How_do_I_search_for_files_and_folders_in_a_specific_location)

## 4. Fuzzy search in Everything 
You need to learn regex.
[How do I use wildcards](https://www.voidtools.com/faq/#How_do_I_use_wildcards)
**Here Is The Content**
> How do I use wildcards?
Using a * in your search will match any number of any type of character.
For example, here is how to search for files and folders that start with e and end with g: e*g
Using a ? in your search will match one character.
For example, here is how to search for files that have a 2 letter file extension: *.??

## 5. Open the file in the contains folder
![08~08_10-47-52.bmp](http://7xpvdr.com1.z0.glb.clouddn.com/08~08_10-47-52.bmp)

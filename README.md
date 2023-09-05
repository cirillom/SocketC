# This is a template project for working with multi file C projects in Linux

#### Folder structure
All your code should be placed inside _src/_ and you can create any numbers of directories needed inside it.
You can also create folders such as _res/_, _scripts/_ in the root folder to aid you.

#### Debugging

In order to use the debugging features this workspace provides you need to install the [C/C++ extension from Microsoft](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools).

To run your code without debugging you can use ``` Ctrl + F5 ``` and to run your code with the debugging tools and breakpoints use ``` F5 ```.


To quickly test run your code without using vscode's debug features you can run the task **C: gcc build and run project** or you can configure a keybind in your _keybindings.json_ using:

```
    {
        "key": "alt+q",
        "command": "workbench.action.tasks.runTask",
        "args": "C: gcc build and run project",
        "when": "editorTextFocus"
    },
```

There's also a task to create a _.c_ and _.h_ files with a base code, you can run it with **C: new .h and .c file**  or you can configure a keybind in your _keybindings.json_ using:
```
    {
        "key": "alt+d",
        "command": "workbench.action.tasks.runTask",
        "args": "C: new .h and .c file",
        "when": "editorTextFocus"
    },
```


##### Planned features
- [ ] Return to old system of creating each .o in order to improve compilation time
- [x] Create header new C file with header

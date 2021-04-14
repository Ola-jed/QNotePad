# QNotePad

[![CodeFactor](https://www.codefactor.io/repository/github/ola-jed/qnotepad/badge)](https://www.codefactor.io/repository/github/ola-jed/qnotepad)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/c795bf59211b4d6ba9d656644b9bb241)](https://www.codacy.com/gh/Ola-jed/QNotePad/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Ola-jed/QNotePad&amp;utm_campaign=Badge_Grade)

QNotePad is a lightweight text editor written in c++ with the qt framework

## How to build?

You need to have [qt](https://www.qt.io/download-qt-installer) installed  to build the app.

```bash
# Compile in debug mode
git clone https://github.com/Ola-jed/QNotePad.git
cd QNotePad
cmake --build cmake-build-debug --target QNotePad -- -j 3
cmake-build-debug/QNotePad # You can pass the path to the file to open as a param
```

Import the project if you use an ide.

### Example
![1](https://user-images.githubusercontent.com/66482155/114748702-13c8ce80-9d4a-11eb-8f00-407dd424425a.png)

### Terminal option
![2](https://user-images.githubusercontent.com/66482155/114748786-280ccb80-9d4a-11eb-98c0-bbfc3e954bb7.png)

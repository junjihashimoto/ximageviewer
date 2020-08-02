# simple image viewer for xwindow

# Usage

```
# Run image viewer with find
> find "image folder" | sort | findviewer
```

# Commands of `findviewer`

| commands | function                                             |
| -------- | --------                                             |
| q        | quit                                                 |
| f        | show next image                                      |
| d        | show previous image                                  |
| s        | skip next 10 images, then show a image               |
| a        | skip previous 10 images, then show a image           |
| v        | skip next 100 images, then show a image              |
| c        | skip previous 100 images, then show a image          |
| x        | skip next 1000 images, then show a image             |
| z        | skip previous 1000 images, then show a image         |
| r        | skip next 10000 images, then show a image            |
| e        | skip previous 10000 images, then show a image        |
| n        | move to the next directory, then show a image        |
| p        | move to the previous directory, then show a image    |
| space    | print the file name of the image currently displayed |

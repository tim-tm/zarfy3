# zarfy3

This repository is a Gtk3 recode of the patched [zarfy](https://github.com/rbtylee/zarfy).

## Features 

zarfy3 is a libxrandr front end. It will soon be able to handle screen resolution, positioning and configuration saving.

## Dependencies

- autotools
- gtk3
- libxrandr
- libxrender

## Setup

### Installing

The [release page](https://github.com/tim-tm/zarfy3/releases) will contain the latest tarballs as soon as zarfy3 is somewhat usable. Go ahead an download the latest version of zarfy3 right there. You may not want to use pre-releases.

Unzipping the tarball (VERSION should be replaced by the actual version you've downloaded)
```bash
tar -xf zarfy3-VERSION.tar.gz && cd zarfy3-VERSION
```

Building zarfy3
```bash
./configure && make
```

Running zarfy3
```bash
./zarfy3
```

Installing zarfy3
```bash
sudo make install
```

### Setting up the development environment
Cloning the repo
```bash
git clone https://github.com/tim-tm/zarfy3.git && cd zarfy3
```

Building
```bash
./build.sh
```

Running
```bash
./build/zarfy3
```

Installing the dev build
```bash
sudo make -C build install
```
or
```bash
cd build && sudo make install
```

## Contribution

Contribute via. GitHub pull requests.
Contributions must be licensed under this project's [license](https://github.com/tim-tm/zarfy3/blob/main/LICENSE).

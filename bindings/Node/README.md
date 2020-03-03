# Node bindings

Read this to learn how to write bindings

## Installation

1. You need to have:
	* Stable Node.js
	* npm (Node package manager) (For ubuntu: `sudo apt-get install npm`)
	* Python version > 2.7

2. Install libpointing and link it in *libpointing/binding.gyp*

3. Install node-addon-api with `npm run install`.

## Compilation

* Run `npm run build`.

## Running

You can run *consoleExample.js* to test the binding with `node consoleExample.js`.

# Server app

You can use libpointing in your browser.
Configure *pointingserver* app or install it with:

	npm install pointingserver

# Windows app

Follow these instructions to build the nw-app for Windows:

1. Compile libpointing in Release x64 mode (root/pointing/pointing.vcxproj)

1. Install everything to build with nw:

	* npm install nw -g
	* npm install nw-gyp -g
	* npm install nw-builder ncp

1. In *libpointing*-folder:
	* run `npm install`
	* run `nw-gyp configure build --target={version}`, where version corresponds to the Node webkit version you want to use (like 0.35.5)

1. In *Node/nw*-folder run `npm install`

1. Run `node builder.js` (Make sure the target is the same).

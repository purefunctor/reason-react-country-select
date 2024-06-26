# reason-react-country-select

`CountrySelect` implemented in `ReasonReact`.

## Demo

### Core Functionality

https://github.com/purefunctor/reason-react-country-select/assets/66708316/57bad2ec-8897-4274-98a2-693808eb19a6

### Virtual Scrolling

https://github.com/purefunctor/reason-react-country-select/assets/66708316/5634fabe-5c2b-48c6-80c2-2755a01d85ee

## Get Started

This project requires an OCaml installation and pnpm for development. To get started: 

```shell
$ opam switch create . --with-test --deps-only -y
$ opam exec -- dune build  # or just `dune build ` with `eval $(opam env)`
$ pnpm install
$ pnpm dev
```

## Keyboard Shortcuts

* `Enter`/`Space`/`ArrowDown` - opens the search combobox when the button is focused.
* `Escape` - closes the search combobox when the input is focused.
* `ArrowUp`/`ArrowDown` - navigates the listbox when the input is focused.
* `Enter` - selects either the first or highlighted country in the listbox.

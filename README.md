# reason-react-country-select

`CountrySelect` implemented in `ReasonReact`.

## Demo



https://github.com/purefunctor/reason-react-country-select/assets/66708316/65bb8f1a-78a3-49b1-9286-8808a3703afa



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
* `Enter` - selects the current country in the listbox when the input is focused.

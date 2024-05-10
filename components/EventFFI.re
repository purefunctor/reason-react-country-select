let document: Dom.element = [%mel.raw "document"];

let elementContainsTarget:
  (~element: Dom.element, ~target: Dom.element) => bool = [%mel.raw
  {|(element, target) => {
      return element.contains(target);
    }|}
];

module Keyboard = {
  type event = {key: string};

  [@mel.send] external preventDefault: event => unit = "preventDefault";

  [@mel.send]
  external addEventListener: (Dom.element, string, event => unit) => unit =
    "addEventListener";

  [@mel.send]
  external removeEventListener: (Dom.element, string, event => unit) => unit =
    "removeEventListener";
};

module Mouse = {
  type event = {target: Dom.element};

  [@mel.send]
  external addEventListener: (Dom.element, string, event => unit) => unit =
    "addEventListener";

  [@mel.send]
  external removeEventListener: (Dom.element, string, event => unit) => unit =
    "removeEventListener";
};

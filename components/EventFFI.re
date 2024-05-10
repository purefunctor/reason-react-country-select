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

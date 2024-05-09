type event = {key: string};

[@mel.send]
external addEventListener: ('element, string, event => unit) => unit =
  "addEventListener";

[@mel.send]
external removeEventListener: ('element, string, event => unit) => unit =
  "removeEventListener";

[@mel.send] external preventDefault: event => unit = "preventDefault";

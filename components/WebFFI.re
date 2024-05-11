module Document = {
  type t = Dom.document;

  external asEventTarget: t => Dom.eventTarget = "%identity";
};

module Element = {
  type t = Dom.element;

  external asEventTarget: t => Dom.eventTarget = "%identity";
  external unsafeAsHtmlElement: t => Dom.htmlElement = "%identity";
  [@mel.send] external contains: (t, ~child: t) => bool = "contains";
};

module HTMLElement = {
  type t = Dom.htmlElement;

  external asEventTarget: t => Dom.eventTarget = "%identity";
  [@mel.send] external focus: t => unit = "focus";
};

module Mouse = {
  type event;

  [@mel.get] external target: event => Dom.element = "target";
  [@mel.send] external preventDefault: event => unit = "preventDefault";

  [@mel.send]
  external addEventListener: (Dom.eventTarget, string, event => unit) => unit =
    "addEventListener";

  [@mel.send]
  external removeEventListener:
    (Dom.eventTarget, string, event => unit) => unit =
    "removeEventListener";
};

external document: Document.t = "document";

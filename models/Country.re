type t = {
  value: string,
  label: string,
};

let getDecodeString = (d: Js.Dict.t(Js.Json.t), k: string): option(string) => {
  Js.Dict.get(d, k)->Option.bind(Js.Json.decodeString);
};

let fromJsonOpt = (json: Js.Json.t): option(t) => {
  let ( let* ) = Option.bind;
  let* country = Js.Json.decodeObject(json);
  let* value = getDecodeString(country, "value");
  let* label = getDecodeString(country, "label");
  Some({value, label});
};

let exampleCountries = [|
  {label: "Japan", value: "jp"},
  {label: "Philippines", value: "ph"},
  {label: "Singapore", value: "sg"},
  {label: "Taiwan", value: "tw"},
  {label: "United States", value: "us"},
|];

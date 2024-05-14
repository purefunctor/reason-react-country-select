module Intl = {
  module NumberFormat = {
    type t;

    type options = {
      notation: string,
      minimumFractionDigits: int,
      maximumFractionDigits: int,
    };

    [@mel.send]
    external format:
      (t, [@mel.unwrap] [ | `float(float) | `int(int)]) => string =
      "format";

    [@mel.scope "Intl"] external make: (string, options) => t = "NumberFormat";
  };
};

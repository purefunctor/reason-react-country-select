module Client = {
  type t;

  [@mel.module "@tanstack/react-query"] [@mel.new]
  external create: unit => t = "QueryClient";
};

module ClientProvider = {
  [@mel.module "@tanstack/react-query"] [@react.component]
  external make:
    (~key: string=?, ~client: Client.t, ~children: React.element=?) =>
    React.element =
    "QueryClientProvider";
};

type queryArguments('data) = {
  queryKey: array(string),
  queryFn: unit => Js.Promise.t('data),
};

// Opaque error type. Use coerceQueryError to coerce to whatever type is used
// in your context. This is unideal, but this simply mirrors Js.Promise.catch.
type queryError;

external coerceQueryError: queryError => 'a = "%identity";

type queryResult('data) = {
  isPending: bool,
  error: Js.Nullable.t(queryError),
  data: Js.Nullable.t('data),
};

[@mel.module "@tanstack/react-query"]
external useQuery: queryArguments('data) => queryResult('data) = "useQuery";

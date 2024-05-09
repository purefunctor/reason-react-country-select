open React.Event;

type state = {
  index: int,
  search: string,
};

type action =
  | GoUp
  | GoDown
  | Search(string);

let reducer = (state: state, action: action) => {
  switch (action) {
  | GoUp => {...state, index: state.index - 1}
  | GoDown => {...state, index: state.index + 1}
  | Search(search) => {index: 0, search}
  };
};

let useInputRef = (onExit, dispatch) => {
  let inputRef = React.useRef(Js.Nullable.null);
  let onKeyDown =
    React.useCallback0((event: KeyboardFFI.event) => {
      switch (event.key) {
      | "Escape" => onExit()
      | "ArrowUp" => dispatch(GoUp)
      | "ArrowDown" => dispatch(GoDown)
      | _ => ()
      }
    });
  React.useEffect(() => {
    switch (inputRef.current |> Js.Nullable.toOption) {
    | None => None
    | Some(inputElement) =>
      KeyboardFFI.addEventListener(inputElement, "keydown", onKeyDown);
      Some(
        () =>
          KeyboardFFI.removeEventListener(inputElement, "keydown", onKeyDown),
      );
    }
  });
  inputRef;
};

[@react.component]
let make = (~onExit, ~onSelect) => {
  let countriesQuery = CountryApi.useCountriesQuery();
  let ({index, search}, dispatch) =
    React.useReducer(reducer, {index: 0, search: ""});
  let inputRef = useInputRef(onExit, dispatch);

  let onChange = event => {
    dispatch(Search(Form.target(event)##value));
  };

  <div>
    <input
      ref={ReactDOM.Ref.domRef(inputRef)}
      placeholder="Search"
      onChange
      value=search
    />
    {switch (countriesQuery) {
     | Pending => <div> {React.string("Loading countries...")} </div>
     | Failed(_) => <div> {React.string("Could not load countries!")} </div>
     | Finished(countryData) =>
       <CountrySelectSearchItems index search onSelect countryData />
     }}
  </div>;
};

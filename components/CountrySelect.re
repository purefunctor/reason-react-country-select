open Models;

let doNothing = _ => ();

[@react.component]
let make =
    (
      ~className: option(string)=?,
      ~country as _: option(string),
      ~onChange: option(string => unit)=?,
    ) => {
  let onChange = onChange |> Option.value(~default=doNothing);

  let (buttonText, setButtonText) = React.useState(() => "United States");
  let (isOpen, setIsOpen) = React.useState(() => true);

  let buttonToggle = _ => {
    setIsOpen(prevIsOpen => !prevIsOpen);
  };

  let onSelect = (country: Country.t) => {
    setIsOpen(_ => false);
    setButtonText(_ => country.label);
    onChange(country.label);
  };

  <div ?className>
    <button onClick=buttonToggle> {React.string(buttonText)} </button>
    {isOpen ? <CountrySelectSearch onSelect /> : React.null}
  </div>;
};

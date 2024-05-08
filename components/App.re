[@react.component]
let make = () => {
  <CountrySelect
    className="custom-class"
    country={Some("us")}
    onChange={country => Js.Console.log(country)}
  />;
};

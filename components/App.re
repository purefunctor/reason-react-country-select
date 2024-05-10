let queryClient = ReactQuery.Client.create();

[@react.component]
let make = () => {
  <ReactQuery.ClientProvider client=queryClient>
    <CountrySelect
      className="custom-class"
      country={Some("us")}
      onChange={country => Js.Console.log(country)}
    />
  </ReactQuery.ClientProvider>;
};

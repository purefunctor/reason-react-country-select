[@mel.module "./App.module.css"] external css: Js.t({..}) = "default";

let queryClient = ReactQuery.Client.create();

[@react.component]
let make = () => {
  <ReactQuery.ClientProvider client=queryClient>
    <main className=css##app>
      <CountrySelect
        className="custom-class"
        country={Some("us")}
        onChange={country => Js.Console.log(country)}
      />
    </main>
  </ReactQuery.ClientProvider>;
};

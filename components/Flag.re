[@react.component]
let make = (~className=?, ~alpha2) => {
  let flagCss = {j|fi fi-$alpha2|j};
  let className =
    switch (className) {
    | None => flagCss
    | Some(className) => flagCss ++ " " ++ className
    };
  <span className />;
};

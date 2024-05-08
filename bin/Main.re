switch (ReactDOM.querySelector("#root")) {
| Some(element) =>
  let root = ReactDOM.Client.createRoot(element);
  let _ = ReactDOM.Client.render(root, <Components.App />);
  ();
| None => ()
};

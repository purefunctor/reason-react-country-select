module ArrowDown = {
  [@react.component]
  let make =
    React.memo(() => {
      <svg
        width="8"
        height="5"
        viewBox="0 0 8 5"
        fill="none"
        xmlns="http://www.w3.org/2000/svg">
        <path
          fillRule="evenodd"
          clipRule="evenodd"
          d="M0 0H8L4 5L0 0Z"
          fill="#333333"
        />
      </svg>
    });
};

module Close = {
  [@react.component]
  let make =
    React.memo(() => {
      <svg
        xmlns="http://www.w3.org/2000/svg"
        width="14"
        height="14"
        viewBox="0 0 24 24">
        <path
          fill="currentColor"
          d="m12 13.4l-4.9 4.9q-.275.275-.7.275t-.7-.275t-.275-.7t.275-.7l4.9-4.9l-4.9-4.9q-.275-.275-.275-.7t.275-.7t.7-.275t.7.275l4.9 4.9l4.9-4.9q.275-.275.7-.275t.7.275t.275.7t-.275.7L13.4 12l4.9 4.9q.275.275.275.7t-.275.7t-.7.275t-.7-.275z"
        />
      </svg>
    });
};

module Search = {
  [@react.component]
  let make =
    React.memo(() => {
      <svg
        width="14"
        height="14"
        viewBox="0 0 14 14"
        fill="none"
        xmlns="http://www.w3.org/2000/svg">
        <path
          fillRule="evenodd"
          clipRule="evenodd"
          d="M6 11C7.01929 11 7.96734 10.695 8.75787 10.1713L12.06 13.47L13.47 12.06L10.1713 8.75783C10.695 7.96731 11 7.01927 11 6C11 3.23858 8.76142 1 6 1C3.23858 1 1 3.23858 1 6C1 8.76142 3.23858 11 6 11ZM9.2 6C9.2 7.76731 7.76731 9.2 6 9.2C4.23269 9.2 2.8 7.76731 2.8 6C2.8 4.23269 4.23269 2.8 6 2.8C7.76731 2.8 9.2 4.23269 9.2 6Z"
          fill="#333333"
        />
      </svg>
    });
};

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

module Search = {
  [@react.component]
  let make =
    React.memo(() => {
      <svg
        width="13"
        height="13"
        viewBox="0 0 13 13"
        fill="none"
        xmlns="http://www.w3.org/2000/svg">
        <path
          fillRule="evenodd"
          clipRule="evenodd"
          d="M5 10C6.01929 10 6.96734 9.695 7.75787 9.17129L11.06 12.47L12.47 11.06L9.17131 7.75783C9.69501 6.96731 10 6.01927 10 5C10 2.23858 7.76142 0 5 0C2.23858 0 0 2.23858 0 5C0 7.76142 2.23858 10 5 10ZM8.2 5C8.2 6.76731 6.76731 8.2 5 8.2C3.23269 8.2 1.8 6.76731 1.8 5C1.8 3.23269 3.23269 1.8 5 1.8C6.76731 1.8 8.2 3.23269 8.2 5Z"
          fill="#333333"
        />
      </svg>
    });
};

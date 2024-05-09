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

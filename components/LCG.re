// Linear Congruential Generator
//
// Used for deterministic pseudo-random number generation.
//
// https://en.wikipedia.org/wiki/Linear_congruential_generator

let initial = 9410;
let m = 990919;
let a = 1909;
let c = 9506;

let makeLcg = () => {
  let seed = ref(initial);
  () => {
    seed := (a * seed^ + c) mod m;
    seed^;
  };
};

let makeLcgRange = (s, e) => {
  let lcg = makeLcg();
  () => {
    let size = float_of_int(e - s);
    let n = float_of_int(lcg()) /. float_of_int(m);
    s + Js.Math.floor_int(n *. size);
  };
};

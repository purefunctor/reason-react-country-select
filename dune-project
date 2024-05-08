(lang dune 3.15)
(using melange 0.1)

(name country_select)

(generate_opam_files true)

(source
 (github purefunctor/reason-react-country-select))

(authors "Justin Garcia")

(maintainers "Justin Garcia")

(license BSD-3-Clause)

(package
 (name country_select)
 (allow_empty)
 (synopsis "A short synopsis")
 (description "A longer description")
 (depends 
  ocaml 
  dune
  (melange (>= "3.0.0-51"))
  (reason (>= "3.11.0"))
  (reason-react (>= "0.14.0"))
  (reason-react-ppx (>= "0.14.0")))
 (tags
  (topics react reasonml component melange)))

; See the complete stanza docs at https://dune.readthedocs.io/en/stable/dune-files.html#dune-project

# uParser
Really Lightweight JSON parser for embedded solutions.

## Disclaimer

uParser is a micro JSON parser aimed for embedded systems that require a little low memory footprint. It will detect some datatypes and also parse them to a defined variable.

## Careful!

It still on development and does not have fallbacks and that much protections.

Will break on object values with commas in it.

For nested objects, you may extract one object from another and then reparse.
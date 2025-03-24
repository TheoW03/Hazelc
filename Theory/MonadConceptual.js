/**
 * just an easy way for me to think about monads
 * JS because JS is a clearn way of understanding *anything*
 */
class Maybe {
    constructor(value) {
        this.value = value;
    }

    // `return` function (or `pure` in functional programming)
    static of(value) {
        return new Maybe(value);
    }

    // `bind` function (or `flatMap` in JavaScript)
    bind(fn) {
        if (this.value === null || this.value === undefined) {
            return this; // Propagate `Nothing` (null or undefined)
        }
        return fn(this.value); // Apply function and return a new Maybe
    }

    // Helper function to check if the value is `Just` or `Nothing`
    isNothing() {
        return this.value === null || this.value === undefined;
    }

    // Extract the value or return a default
    getOrElse(defaultValue) {
        return this.isNothing() ? defaultValue : this.value;
    }
}
// Define some functions to use with the Maybe monad
const increment = (x) => Maybe.of(x + 1);  // Add 1 to the value
const double = (x) => Maybe.of(x * 2);    // Double the value
const f = (x) => Maybe.of(null);    // Double the value

// Start with a valid value wrapped in a Maybe
let result = Maybe.of(5)
    .bind(f)  // Increment the value inside Maybe
    .bind(double);    // Double the value inside Maybe
console.log(result)
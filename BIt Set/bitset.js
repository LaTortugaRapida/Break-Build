class BitSet {
    #capacity;
    #arr;

    constructor(capacity = 32) {
        if (!Number.isInteger(capacity) || capacity < 0) {
            throw new Error("Capacity must be a positive integer!");
        }
        if (capacity < 32) {
            this.#arr = new Uint32Array(1);
        } else {
            this.#arr = new Uint32Array(Math.ceil(capacity / 32));
            this.#capacity = capacity;
        }
        this.#arr.fill(0);
    }

    set(index) {
        if (!Number.isInteger(index) || index < 0) {
            throw new Error("Index must be a positive integer!");
        }
        if (index > this.#capacity * 32) {
            throw new Error("Invalid index!");
        }
        let bucket = Math.floor(index / 32);
        let absoluteIdx = index % 32;

        this.#arr[bucket] |= 1 << absoluteIdx;
    }

    reset (index) {
        if (!Number.isInteger(index) || index < 0) {
            throw new Error("Index must be a positive integer!");
        }
        if (index > this.#capacity * 32) {
            throw new Error("Invalid index!");
        }
        let bucket = Math.floor(index / 32);
        let absoluteIdx = index % 32;
        let tmp = this.#arr[bucket] | (1 << absoluteIdx);
        
        if(this.#arr[bucket] === tmp) {
            this.#arr[bucket] ^= 1 << absoluteIdx;
        }
    }

    toString() {
    return [...this.#arr];
}
}


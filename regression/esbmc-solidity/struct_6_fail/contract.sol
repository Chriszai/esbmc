pragma solidity ^0.8.0;

contract MyContract {
    struct T {
        uint8[4] x;
    }
    function callVuln() public {
        uint8[4] memory xx = [uint8(1), 2, 3, 4];
        T memory tInstance = T(xx);
        assert(tInstance.x[0] < 0); // fail
    }
}

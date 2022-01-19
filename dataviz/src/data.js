let gcc = [
	1213, 926, 883, 840, 782, 735, 665, 666, 634, 622, 585, 589, 567, 555, 555,
	547,
].map((v, idx) => ({ time: v, cores: idx + 2 }));

let llvm = [
	1291, 910, 1076, 793, 671, 711, 687, 658, 633, 619, 579, 561, 576, 548, 550,
	547,
].map((v, idx) => ({ time: v, cores: idx + 2 }));

let gccLto = [
	1077, 923, 884, 781, 632, 671, 606, 601, 578, 544, 539, 518, 519, 512, 506,
	504,
].map((v, idx) => ({ time: v, cores: idx + 2 }));

let llvmLto = [
	954, 940, 818, 707, 680, 590, 590, 563, 553, 544, 530, 509, 506, 503, 494,
	496,
].map((v, idx) => ({ time: v, cores: idx + 2 }));

let noAddsLlvmLto = [
	1044, 998, 787, 697, 676, 608, 565, 553, 537, 525, 505, 495, 483, 476, 475,
	468,
].map((v, idx) => ({ time: v, cores: idx + 2 }));

export default { gcc, llvm, llvmLto, gccLto, noAddsLlvmLto };

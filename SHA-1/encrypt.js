const sha1 = require("./src/sha1");
const clc = require("cli-color");

process.stdout.write(clc.white.bold("평문 입력 :  "));

process.stdin.on("data", function (data) {
  const val = data.toString().trim();

  process.stdout.write(
    clc.white.bold("해쉬된 값 : ") + clc.red.bold(sha1(val))
  );
  process.stdout.write(
    clc.white.bold(
      "\n-----------------------------------------------------\n평문 입력 :  "
    )
  );
});

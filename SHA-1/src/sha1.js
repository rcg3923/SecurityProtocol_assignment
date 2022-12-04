const utils = require("./utils");

function sha1(text) {
  //기본적으로 정의되어있는 16진수를 2진수로 치환한 값
  let h0 = "01100111010001010010001100000001";
  let h1 = "11101111110011011010101110001001";
  let h2 = "10011000101110101101110011111110";
  let h3 = "00010000001100100101010001110110";
  let h4 = "11000011110100101110000111110000";

  //문자형 - > 아스키코드로
  const asciiText = text.split("").map((letter) => utils.charToASCII(letter));

  //변환된 아스키코드 값을 8비트 형식으로 구성 앞에 비워있는 공간 맨앞에 0을 채워줌
  let binary8bit = asciiText
    .map((num) => utils.asciiToBinary(num))
    .map((num) => utils.padZero(num, 8));

  //sha1 알고리즘에 따라 8비트씩 끊어서 연결한 것 맨 뒤에 1을 추가해줌
  let numString = binary8bit.join("") + "1";

  // 448이 될때까지 0을 채워넣어주는 것
  while (numString.length % 512 !== 448) {
    numString += "0";
  }

  // 기존 문자열에 64비트가 0이 되도록 추가하는 과정
  const length = binary8bit.join("").length;
  const binaryLength = utils.asciiToBinary(length);

  //SHA-1 은 64비트 이상의 문자열은 지원하지 않는다.
  const paddedBinLength = utils.padZero(binaryLength, 64);
  numString += paddedBinLength;

  //만들어진 512비트를 여러개로 쪼개는 과정
  const chunks = utils.stringSplit(numString, 512);

  const words = chunks.map((chunk) => utils.stringSplit(chunk, 32));

  // 16개로 나눠진 배열에 청크를 반복하여 xor 연산을 사용해서 80개로 확장하는 과정
  const words80 = words.map((array) => {
    for (let i = 16; i <= 79; i++) {
      //4단어씩 가져오는 과정
      const wordA = array[i - 3];
      const wordB = array[i - 8];
      const wordC = array[i - 14];
      const wordD = array[i - 16];

      //각 단어마다 연속으로 비트 연산
      const xorA = utils.xOR(wordA, wordB);
      const xorB = utils.xOR(xorA, wordC);
      const xorC = utils.xOR(xorB, wordD);

      //LEFT LOTATE
      const leftRotated = utils.leftRotate(xorC, 1);
      //암호화 작업이 끝난 것을 배열에 추가
      array.push(leftRotated);
    }
    return array;
  });

  // 암호화된 과정에서 또 한번 sha-1 알고리즘에 의해 재할당 진행
  for (let i = 0; i < words80.length; i++) {
    //H0, h1, h2, h3, h4 는 초기에 헥사값으로 샤원에서 제공해주는 것을 이용하여 각각 a,b,c,d,e 에 할당
    let a = h0;
    let b = h1;
    let c = h2;
    let d = h3;
    let e = h4;
    //루프를 80회 반복, 샤원 알고리즘에 의한  1-20, 20-39 등등 정해진 과정에 따라 비트연산을 수행해서 암호화한다.
    for (let j = 0; j < 80; j++) {
      let f;
      let k;
      if (j < 20) {
        const BandC = utils.and(b, c);
        const notB = utils.and(utils.not(b), d);
        f = utils.or(BandC, notB);
        k = "01011010100000100111100110011001";
      } else if (j < 40) {
        const BxorC = utils.xOR(b, c);
        f = utils.xOR(BxorC, d);
        k = "01101110110110011110101110100001";
      } else if (j < 60) {
        const BandC = utils.and(b, c);
        const BandD = utils.and(b, d);
        const CandD = utils.and(c, d);
        const BandCorBandD = utils.or(BandC, BandD);
        f = utils.or(BandCorBandD, CandD);
        k = "10001111000110111011110011011100";
      } else {
        const BxorC = utils.xOR(b, c);
        f = utils.xOR(BxorC, d);
        k = "11001010011000101100000111010110";
      }

      const word = words80[i][j];
      const tempA = utils.binaryAddition(utils.leftRotate(a, 5), f);
      const tempB = utils.binaryAddition(tempA, e);
      const tempC = utils.binaryAddition(tempB, k);
      let temp = utils.binaryAddition(tempC, word);

      temp = utils.truncate(temp, 32);
      e = d;
      d = c;
      c = utils.leftRotate(b, 30);
      b = a;
      a = temp;
    }

    h0 = utils.truncate(utils.binaryAddition(h0, a), 32);
    h1 = utils.truncate(utils.binaryAddition(h1, b), 32);
    h2 = utils.truncate(utils.binaryAddition(h2, c), 32);
    h3 = utils.truncate(utils.binaryAddition(h3, d), 32);
    h4 = utils.truncate(utils.binaryAddition(h4, e), 32);
  }
  // 2진수로 전환되었던 값들을 다시 16진수로 표현하여 최종적으로 해시값을 반환한다.
  return [h0, h1, h2, h3, h4]
    .map((string) => utils.binaryToHex(string))
    .join("");
}

module.exports = sha1;

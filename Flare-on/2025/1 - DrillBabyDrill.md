
Try the pygame first.
```python
python DrillBabyDrill.py
```
# Game Logic
- The player can only move to left or right (x-co-ordination)
- The level displayed on the screen
- `Loc` is the current position of the player (`x`)
- `Depth` is how deep the drill goes

![[2025-1-drill.png]]
When you hit the boulder, game over.
![[2025-1-drill (4).png]]
# Code Examination
Here are several key parts I think it's important to find the flag.

First, the level will be displayed on the screen.
![[2025-1-drill (1).png]]
Here are the key functions:
- How the boulder was put?
- How to hit the bear?
![[2025-1-drill (2).png]]
Then we can have the flag?
![[2025-1-drill (3).png]]

Then, go to line 213 - 217, which calculated the boulder position

![[2025-1-drill (5).png]]
If the tile (`i`, x in this case) isn't equal to the length of current level name, boulder will can placed (x, y) (y is randomly assigned). 

So, bear will be put in the condition that (`i` == the length of current level name ).

# Level Names
- The level names and their lengths are:
    - California: 10
    - Ohio: 4
    - Death Valley: 12
    - Mexico: 6
    - The Grand Canyon: 16


![[2025-1-drill (8).png]]
# Flag
```python
  

def main():
    bearsum = 4 * 6 * 12 * 10 * 16
    key = bearsum >> 8
    encoded = "\xd0\xc7\xdf\xdb\xd4\xd0\xd4\xdc\xe3\xdb\xd1\xcd\x9f\xb5\xa7\xa7\xa0\xac\xa3\xb4\x88\xaf\xa6\xaa\xbe\xa8\xe3\xa0\xbe\xff\xb1\xbc\xb9"
    print(f"length of `encoded` is {len(encoded)}\n")
    plaintext = []

    for i in range(0, len(encoded)):
        plaintext.append(chr(ord(encoded[i]) ^ (key+i)))

    print(f"{''.join(plaintext)}")

  

if __name__ == '__main__':
    main()
```
![[2025-1-drill (6).png]]
# Recap
- Each level name has a specific length that determines the safe drilling column
- When you find a bear at position `x`, the code multiplies: `bear_sum *= player.x`
- Final `bear_sum` = 10 × 4 × 12 × 6 × 16 = 46,080 (0xB400 in hex)
- The `GenerateFlagText()` function:
    - Takes the `bear_sum` and shifts it right 8 bits: `key = 46080 >> 8 = 0xB4 = 180`
    - XORs each byte of the encoded string with `(key + index)`
- The flag is `drilling_for_teddies@flare-on.com`
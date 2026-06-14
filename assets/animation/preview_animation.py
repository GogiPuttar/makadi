#!/usr/bin/env python3

import argparse
from pathlib import Path

import cv2


def main():
    parser = argparse.ArgumentParser(
        description="Preview a sprite animation from a folder of images."
    )

    parser.add_argument(
        "folder",
        help="Folder containing animation frames",
    )

    parser.add_argument(
        "--fps",
        type=float,
        default=12.0,
        help="Playback frame rate (default: 12)",
    )

    parser.add_argument(
        "--loop",
        action="store_true",
        help="Loop forever",
    )

    args = parser.parse_args()

    folder = Path(args.folder)

    extensions = {
        ".png",
        ".jpg",
        ".jpeg",
        ".bmp",
        ".webp",
    }

    frames = sorted(
        p for p in folder.iterdir()
        if p.suffix.lower() in extensions
    )

    if not frames:
        raise RuntimeError(
            f"No image files found in {folder}"
        )

    delay_ms = max(1, int(1000.0 / args.fps))

    print(f"Loaded {len(frames)} frames")
    print(f"Playback: {args.fps:.1f} FPS")
    print("Press ESC to quit")

    while True:
        for frame_path in frames:
            image = cv2.imread(str(frame_path), cv2.IMREAD_UNCHANGED)

            if image is None:
                print(f"Failed to load: {frame_path}")
                continue

            cv2.imshow("Makadi Animation Preview", image)

            key = cv2.waitKey(delay_ms)

            if key == 27:  # ESC
                cv2.destroyAllWindows()
                return

        if not args.loop:
            break

    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
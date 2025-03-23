"""
Script for uploading progress to the frogress.

Adapted from https://github.com/zeldaret/af/blob/aeb01dcb95e8281f89f355604dbeba519ef073d5/tools/progress.py
MIT License: https://opensource.org/license/mit
"""
from pathlib import Path
from datetime import datetime
import argparse
import mapfile_parser

ASMPATH = Path("asm")
NONMATCHINGS = "nonmatchings"
BASE_URL = "https://progress.deco.mp"
SLUG = "sotc"
VERSION = "preview"

def getProgressFromMapFile(mapFile: mapfile_parser.MapFile, asmPath: Path, nonmatchings: Path, aliases: dict[str, str]=dict(), pathIndex: int=2) -> tuple[mapfile_parser.ProgressStats, dict[str, mapfile_parser.ProgressStats]]:
    totalStats = mapfile_parser.ProgressStats()
    progressPerFolder: dict[str, mapfile_parser.ProgressStats] = dict()

    for segment in mapFile:
        for file in segment:
            if len(file) == 0:
                continue

            folder = file.filepath.parts[pathIndex]

            if ".a" in folder:
                folder = folder.split('.a')[0]

            if folder in aliases:
                folder = aliases[folder]

            if folder not in progressPerFolder:
                progressPerFolder[folder] = mapfile_parser.ProgressStats()

            originalFilePath = Path(*file.filepath.parts[pathIndex:])

            extensionlessFilePath = originalFilePath
            while extensionlessFilePath.suffix:
                extensionlessFilePath = extensionlessFilePath.with_suffix("")

            fullAsmFile = asmPath / extensionlessFilePath.with_suffix(".s")

            handwrittenAsmFiles = [Path("sdk/crt0.o")]

            if originalFilePath in handwrittenAsmFiles:
                wholeFileIsUndecomped = False
            else:
                wholeFileIsUndecomped = fullAsmFile.exists()

            for func in file:
                funcAsmPath = nonmatchings / extensionlessFilePath / f"{func.name}.s"

                symSize = 0
                if func.size is not None:
                    symSize = func.size

                if wholeFileIsUndecomped:
                    totalStats.undecompedSize += symSize
                    progressPerFolder[folder].undecompedSize += symSize
                elif funcAsmPath.exists():
                    totalStats.undecompedSize += symSize
                    progressPerFolder[folder].undecompedSize += symSize
                else:
                    totalStats.decompedSize += symSize
                    progressPerFolder[folder].decompedSize += symSize

    return totalStats, progressPerFolder

def getProgress(mapPath: Path, asmPath: str) -> tuple[mapfile_parser.ProgressStats, dict[str, mapfile_parser.ProgressStats]]:
    """
    Gets the progress of the project using the mapfile parser.
    """
    mapFile = mapfile_parser.MapFile()
    mapFile.readMapFile(mapPath)

    for segment in mapFile:
        for file in segment:
            if len(file) == 0:
                continue

            filepathParts = list(file.filepath.parts)
            file.filepath = Path(*filepathParts)

    asmPath = Path(ASMPATH / asmPath)

    nonMatchingsPath = asmPath / NONMATCHINGS

    progress = getProgressFromMapFile(mapFile.filterBySectionType(".text"), asmPath, nonMatchingsPath, aliases={"ultralib": "libultra"})

    return progress

def processMapFiles(mapFiles: list[(str,str)], frogress_api_key: str) -> None:
    """
    Processes a list of map files and uploads their progress to frogress.
    """
    for mapFile in mapFiles:
        print(f"Processing map file: {mapFile[0]}")
        mapfilePathStr = mapFile[0]
        mapfileAsmDir = mapFile[1]
        codeTotalStats, codeProgressPerFolder = getProgress(mapfilePathStr, mapfileAsmDir)
        codeEntries: dict[str, int] = mapfile_parser.frontends.upload_frogress.getFrogressEntriesFromStats(codeTotalStats, codeProgressPerFolder, verbose=True)

        mapfile_parser.progress_stats.printStats(codeTotalStats, codeProgressPerFolder)

        url = mapfile_parser.utils.generateFrogressEndpointUrl(BASE_URL, SLUG, VERSION)
        mapfile_parser.frontends.upload_frogress.uploadEntriesToFrogress(codeEntries, "default", url, apikey=frogress_api_key, verbose=True)

def main(args: argparse.ArgumentParser) -> None:
    """
    Main function, calculates the progress and uploads it to frogress.
    """
    frogress_api_key = args.frogress_api_key
    if not frogress_api_key:
        raise ValueError("Missing frogress API key.")

    # Map files and their asm directories
    mapFiles = [
        ("build/SCPS_150.97.map", "loader"),
        ("build/KERNEL.XFF.map", "kernel")
    ]

    processMapFiles(mapFiles, frogress_api_key)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Upload progress to the frogress")
    parser.add_argument("--frogress_api_key", help="API key for the frogress")

    args = parser.parse_args()
    main(args)
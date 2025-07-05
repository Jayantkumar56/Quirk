
import sys
import os
import shutil
from pathlib import Path


def ValidateDirectoryPath(pathStr: str, checkWritePermission: bool = False) -> Path:
    try:
        p = Path(pathStr).resolve()

        # Existence Check
        if not p.exists():
            raise ValueError(f"Path does not exist: '{pathStr}' (resolved to '{p}')")

        # Type Check (Is it a directory?)
        if not p.is_dir():
            raise ValueError(f"Path is not a directory: '{pathStr}' (resolved to '{p}')")

        # Permissions Check (Read permission is almost always needed for directories)
        if not os.access(p, os.R_OK):
            raise ValueError(f"No read permission for directory: '{pathStr}' (resolved to '{p}')")

        # Write permission check
        if checkWritePermission and not os.access(p, os.W_OK):
            raise ValueError(f"No write permission for directory: '{pathStr}' (resolved to '{p}')")

        # Execute permission (needed to traverse/list contents on some systems)
        if not os.access(p, os.X_OK):
             raise ValueError(f"No execute (traverse) permission for directory: '{pathStr}' (resolved to '{p}')")

        return p

    except Exception as e:
        raise ValueError(f"Failed to validate path '{pathStr}': {e}") from e


def SynchronizeAssets(assetDirectory: Path, toCopyDirectory: Path) :
    print(f"Copying assets to output directory")
    print(f"Source assets directory: {assetDirectory}")
    print(f"Destination assets directory: {toCopyDirectory}")

    # Ensure the source assets directory exists and is a directory
    try:
        assetDirectory = ValidateDirectoryPath(str(assetDirectory))
    except ValueError as e:
        print(f"Error: Source assets directory issue: {e}")
        sys.exit(1)

    # Ensure the destination directory exists or create it
    try:
        toCopyDirectory.mkdir(parents=True, exist_ok=True)
        # After creation, validate its permissions for writing
        toCopyDirectory = ValidateDirectoryPath(str(toCopyDirectory), checkWritePermission=True)
    except ValueError as e:
        print(f"Error creating/validating destination directory: {e}")
        sys.exit(1)
    except OSError as e:
        print(f"OS Error creating destination directory '{toCopyDirectory}': {e}")
        sys.exit(1)

    print("--- Starting Asset Synchronization ---")

    source_files = {} # Stores {relative_path: full_path} for source files
    dest_files = {}   # Stores {relative_path: full_path} for destination files

    # --- Scan Source Directory and Copy/Update Files ---
    for dirpath, dirnames, filenames in os.walk(assetDirectory):
        currentSourceDir = Path(dirpath)
        relative_path_to_source_dir = currentSourceDir.relative_to(assetDirectory)
        currentDestDir = toCopyDirectory / relative_path_to_source_dir

        # Ensure corresponding destination subdirectories exist
        currentDestDir.mkdir(parents=True, exist_ok=True)

        for filename in filenames:
            sourceFilePath = currentSourceDir / filename
            destFilePath = currentDestDir / filename
            relativeFilePath = sourceFilePath.relative_to(assetDirectory)

            source_files[relativeFilePath] = sourceFilePath

            copy_needed = False
            if not destFilePath.exists():
                print(f"  NEW: {relativeFilePath}")
                copy_needed = True
            else:
                try:
                    source_stat = sourceFilePath.stat()
                    dest_stat = destFilePath.stat()

                    # Compare modification time and size for efficiency
                    if source_stat.st_mtime > dest_stat.st_mtime or source_stat.st_size != dest_stat.st_size:
                        print(f"  MODIFIED: {relativeFilePath}")
                        copy_needed = True
                except FileNotFoundError:
                    # Should not happen if destFilePath.exists() is true, but defensive
                    print(f"  Warning: Destination file {destFilePath} vanished during check. Will copy.")
                    copy_needed = True
                except OSError as e:
                    print(f"  Error comparing files {sourceFilePath} and {destFilePath}: {e}")
                    continue # Skip this file

            if copy_needed:
                try:
                    shutil.copy2(sourceFilePath, destFilePath) # copy2 preserves metadata like mtime
                    print(f"    Copied: {sourceFilePath} -> {destFilePath}")
                except Exception as e:
                    print(f"    ERROR copying {sourceFilePath} to {destFilePath}: {e}")

    # --- Scan Destination Directory and Delete Removed Files ---
    for dirpath, dirnames, filenames in os.walk(toCopyDirectory):
        currentDestDir = Path(dirpath)
        relative_path_to_dest_dir = currentDestDir.relative_to(toCopyDirectory)

        for filename in filenames:
            destFilePath = currentDestDir / filename
            relativeFilePath = destFilePath.relative_to(toCopyDirectory)

            # Check if this file exists in our source_files map
            if relativeFilePath not in source_files:
                print(f"  DELETED: {relativeFilePath}")
                try:
                    os.remove(destFilePath)
                    print(f"    Deleted: {destFilePath}")
                except OSError as e:
                    print(f"    ERROR deleting {destFilePath}: {e}")

    # --- Clean up Empty Directories in Destination ---
    # This needs a bottom-up approach, so we can't do it directly in the os.walk above.
    # We'll walk again, but explicitly check if directories are empty.
    # Note: shutil.rmtree would remove everything, which is not what we want for sync.
    # We want to remove only *empty* directories.

    # Walk bottom-up to ensure empty subdirectories are removed before their parents
    for dirpath, dirnames, filenames in os.walk(toCopyDirectory, topdown=False):
        currentDestDir = Path(dirpath)
        # Don't try to remove the root 'toCopyDirectory' itself unless it becomes empty
        if currentDestDir == toCopyDirectory:
            continue

        try:
            # Check if the directory is empty (no files and no subdirectories left from our walk)
            # We can use iterdir and check if it's empty
            if not any(currentDestDir.iterdir()):
                currentDestDir.rmdir() # rmdir only removes empty directories
                print(f"  Removed empty directory: {currentDestDir}")
        except OSError as e:
            # This can happen if another process created something there, or permissions
            print(f"  ERROR removing empty directory {currentDestDir}: {e}")

    print("--- Asset Synchronization Complete ---")



# Ideally this would copy the file only once 
def TryCopyfile(filename: str, srcDir: Path, dstDir: Path) :
    try:
        srcDir    = ValidateDirectoryPath(str(srcDir), checkWritePermission = True)
        dstDir = ValidateDirectoryPath(str(dstDir), checkWritePermission = True)
    except ValueError as e:
        print(f"    copying {filename} :- Error validating build directory: {e}")
        return
    except Exception as e:

        print(f"    copying {filename} :- Error An unexpected error occurred during build directory validation: {e}")
        return

    sourceFilePath = srcDir / filename
    destFilePath = dstDir / filename

    copy_needed = False

    if not destFilePath.exists():
        print(f"  NEW: {destFilePath}")
        copy_needed = True
    else:
        sourceStat = sourceFilePath.stat()
        destStat = destFilePath.stat()

        # Compare modification time
        if sourceStat.st_mtime > destStat.st_mtime:
            print(f"  MODIFIED: {destFilePath}")
            copy_needed = True

    if copy_needed:
        try:
            shutil.copy2(sourceFilePath, destFilePath) # copy2 preserves metadata like mtime
            print(f"    Copied: {sourceFilePath} -> {destFilePath}")
        except Exception as e:
            print(f"    ERROR copying {sourceFilePath} to {destFilePath}: {e}")
    else:
        print(f"    {filename} is already up-to-date.")


if __name__ == "__main__":
    if len(sys.argv) > 2:
        print("Usage: python PostBuild.py <buildDirectory>")
        sys.exit(1)

    # If exeDirectory do not exists it creates it
    p = Path(sys.argv[1]).resolve()
    p.mkdir(parents=True, exist_ok=True)

    # Validate the build directory provided via command line
    try:
        exeDirectory = ValidateDirectoryPath(sys.argv[1], checkWritePermission = True)
    except ValueError as e:
        print(f"Error validating build directory: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred during build directory validation: {e}")
        sys.exit(1)

    # Get the path to the current script's directory
    scriptDir = Path(__file__).parent.resolve()

    assetDirectory = (scriptDir / ".." / "assets").resolve()
    toCopyDirectory = exeDirectory / "assets"
    SynchronizeAssets(assetDirectory, toCopyDirectory)

    # Provide "RecentProjects.yaml"
    RecentProjectsSrcDir = (scriptDir / "..").resolve()
    RecentProjectsDstcDir = exeDirectory
    TryCopyfile("RecentProjects.yaml", RecentProjectsSrcDir, RecentProjectsDstcDir)

    # Provide "imgui.ini"
    imguiSrcDir = (scriptDir / "..").resolve()
    imguiDstcDir = exeDirectory
    TryCopyfile("imgui.ini", imguiSrcDir, imguiDstcDir)

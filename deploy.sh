#!/bin/bash
set -e # Exit with nonzero exit code if anything fails

SOURCE_BRANCH="master"
TARGET_BRANCH="gh-pages"
WLIB_BRANCH="library"
WLIB_REPO="https://github.com/waterloop/wlib.git"

# Pull requests and commits to other branches shouldn't try to deploy, just build to verify
if [ "$TRAVIS_PULL_REQUEST" != "false" -o "$TRAVIS_BRANCH" != "$SOURCE_BRANCH" ]; then
    echo "Skipping deploy"
    exit 0
fi

# Save some useful information
REPO=`git config remote.origin.url`
SSH_REPO=${REPO/https:\/\/github.com\//git@github.com:}
SHA=`git rev-parse --verify HEAD`

echo "Pushing to gh-pages"

# Clone the existing gh-pages for this repo into docs_out
# Create a new empty branch if gh-pages doesn't exist yet (should only happen on first deply)
git clone $REPO docs_out
cd docs_out

git checkout $TARGET_BRANCH || git checkout --orphan $TARGET_BRANCH
cd ..

echo "Removing current files"

# Clean out existing contents
rm -rf docs_out/*
cd docs

# Run our compile script
doxygen doxygen.conf

# Move content from html folder
cp -r html/. ../docs_out
cp ../.nojekyll ../docs_out

# Now let's go have some fun with the cloned repo
cd ../docs_out
git config user.name "Travis CI"
git config user.email "deep@deepdhillon.ca"

echo "Deploying to gh-pages"

# Commit the "changes", i.e. the new version.
# The delta will show diffs between new and old versions.
git add .
git commit -m "Deploy to GitHub Pages: ${SHA}"

# Get the deploy key by using Travis's stored variables to decrypt deploy_key.enc
ENCRYPTED_KEY_VAR="encrypted_${ENCRYPTION_LABEL}_key"
ENCRYPTED_IV_VAR="encrypted_${ENCRYPTION_LABEL}_iv"
ENCRYPTED_KEY=${!ENCRYPTED_KEY_VAR}
ENCRYPTED_IV=${!ENCRYPTED_IV_VAR}
openssl aes-256-cbc -K $ENCRYPTED_KEY -iv $ENCRYPTED_IV -in ../deploy_key.enc -out deploy_key -d
chmod 600 deploy_key
eval `ssh-agent -s`
ssh-add deploy_key
rm deploy_key

# Now that we're all set up, we can push.
git push $SSH_REPO $TARGET_BRANCH

cd ..

echo "Deployed to gh-pages"

# Clone the existing repo (library branch)
# Create a new empty branch if library doesn't exist yet (should only happen on first deply)
git clone $REPO library
cd library
git checkout $WLIB_BRANCH || git checkout --orphan $WLIB_BRANCH
cd ..

# Clean out existing contents and copy new content
rm -rf library/*
cp -r lib/wlib/. library
cp README.md library
cp .gitignore library
cp LICENSE library

cd library
git add .
git commit -m "Deploying Wlib Library: ${SHA}"
git push $SSH_REPO $WLIB_BRANCH

echo "Deployed Wlib library (branch)"
cd ..

# Clone the existing wlib repo for this repo
git clone $WLIB_REPO wlib-repo

# Clean out existing contents and copy new content
rm -rf wlib-repo/*
cp -r lib/wlib/. wlib-repo
cp README.md wlib-repo
cp .gitignore wlib-repo
cp LICENSE wlib-repo

cd wlib-repo
git add .
git commit -m "Deploying Wlib Library (repo): ${SHA}"
git push $SSH_REPO $WLIB_BRANCH

echo "Deployed Wlib library (repo)"
echo "Deployed Wlib library on wlib repo and library branch"

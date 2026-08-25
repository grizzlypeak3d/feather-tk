# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

# Embed a text file in a generated header as a string constant:
#
#     ftk_embed_text(<header> <input> <variable>)
#
# The header defines "inline const char* <variable>" holding the file's
# contents. This is for text resources -- a JSON widget layout, a
# shader, a template -- where the file stays the source of truth: the
# same file can be edited live with the preview example and shipped
# inside the binary, with no copy to forget. Binary resources go
# through ftk-resource instead.
#
# The file is re-read when it changes by way of a configure dependency.
# The contents are wrapped in a raw string literal with the delimiter
# "ftkembed", which the text must not contain.
function(ftk_embed_text HEADER INPUT VARIABLE)
    file(READ ${INPUT} ftk_embed_text_CONTENT)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${INPUT})
    file(CONFIGURE
        OUTPUT ${HEADER}
        CONTENT "#pragma once

inline const char* @VARIABLE@ = R\"ftkembed(@ftk_embed_text_CONTENT@)ftkembed\";
"
        @ONLY)
endfunction()

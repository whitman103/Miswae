from __future__ import annotations
import compress
import typing

__all__ = [
    "CompressionInterface"
]


class CompressionInterface():
    def __init__(self, arg0: int) -> None: ...
    def compressVector(self, *, vector: typing.List[int]) -> bytes: ...
    def decompressVector(self, *, compressed_string: bytes) -> typing.List[int]: ...
    def testBackend(self) -> None: ...
    @property
    def symbolTable(self) -> typing.Dict[int, typing.Tuple[int, int]]:
        """
        :type: typing.Dict[int, typing.Tuple[int, int]]
        """
    @symbolTable.setter
    def symbolTable(self, arg0: typing.Dict[int, typing.Tuple[int, int]]) -> None:
        pass
    @property
    def totalCount(self) -> int:
        """
        :type: int
        """
    pass
